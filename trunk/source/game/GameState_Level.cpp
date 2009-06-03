/***
 * hesperus: GameState_Level.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "GameState_Level.h"

#include <algorithm>
#include <iostream>
#include <list>

#include <SDL.h>

#include <source/gui/ExplicitLayout.h>
#include <source/gui/Picture.h>
#include <source/gui/Screen.h>
#include <source/io/files/LevelFile.h>
#include <source/io/util/DirectoryFinder.h>
#include <source/math/geom/GeomUtil.h>
#include <source/level/LevelViewer.h>
#include <source/level/objects/base/ObjectCommand.h>
#include <source/level/objects/components/ICmpAABBBounds.h>
#include <source/level/objects/components/ICmpActivatable.h>
#include <source/level/objects/components/ICmpModelRender.h>
#include <source/level/objects/components/ICmpPhysics.h>
#include <source/level/objects/components/ICmpPosition.h>
#include <source/level/objects/components/ICmpYoke.h>
#include <source/level/objects/MoveFunctions.h>
namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
GameState_Level::GameState_Level(const std::string& levelFilename)
:	m_inputGrabbed(false)
{
	m_level = LevelFile::load(levelFilename);
}

//#################### PUBLIC METHODS ####################
void GameState_Level::enter()
{
	// Clear all pending SDL events before we get started.
	{ SDL_Event e; while(SDL_PollEvent(&e)) {} }

	set_display(construct_display());
	grab_input();
}

void GameState_Level::leave()
{
	ungrab_input();
}

GameState_Ptr GameState_Level::update(int milliseconds, UserInput& input)
{
	// TEMPORARY: Allow quick toggling of the input grab using the 'g' key (for debugging purposes).
	if(input.key_down(SDLK_g))
	{
		input.release_key(SDLK_g);
		if(m_inputGrabbed) ungrab_input();
		else grab_input();
	}

	do_yokes(milliseconds, input);
	do_physics(milliseconds);
	do_animations(milliseconds);
	do_activatables(input);

	return GameState_Ptr();
}

//#################### PRIVATE METHODS ####################
GUIComponent_Ptr GameState_Level::construct_display()
{
	GUIContainer<ExplicitLayout> *display = new GUIContainer<ExplicitLayout>;

	bf::path imagesDir = determine_images_directory(determine_base_directory_from_game());

	const Screen& screen = Screen::instance();
	int width = screen.dimensions().width();
	int height = screen.dimensions().height();
	display->layout().add(new Picture((imagesDir / "title.png").file_string()), Extents(width/4, 0, width*3/4, width/8));
	display->layout().add(new LevelViewer(m_level), Extents(50, 200, width - 50, height - 50));
	GUIContainer<ExplicitLayout> *cont = new GUIContainer<ExplicitLayout>;
	cont->layout().add(new Picture((imagesDir / "title.png").file_string()), Extents(500, 0, 700, 50));
	display->layout().add(cont, Extents(100, 100, 200, 200));

	return GUIComponent_Ptr(display);
}

void GameState_Level::do_activatables(UserInput& input)
{
	// Step 1:	Set all activatable objects to be unhighlighted when rendered.
	const ObjectManager_Ptr& objectManager = m_level->object_manager();
	std::vector<ObjectID> activatables = objectManager->group("Activatables");
	for(size_t i=0, size=activatables.size(); i<size; ++i)
	{
		ICmpModelRender_Ptr cmpRender = objectManager->get_component(activatables[i], cmpRender);
		cmpRender->set_highlights(false);
	}

	// Step 2:	Find the half-ray representing the player's line of sight.

	// FIXME: This only works when the camera is attached to the player.
	Camera_Ptr camera = m_level->camera();
	camera->update();
	Vector3d eye = camera->eye(), look = camera->look();

	// Step 3:	Check all activatable objects in leaves in the PVS of the viewer's leaf and find the nearest within range (if any)
	//			whose AABB is intersected by the half-ray. (If there's no such object, return.)
	ObjectID nearestObject;
	double nearestDistSquared = INT_MAX;

	std::vector<int> visibleLeaves = m_level->find_visible_leaves(eye);
	for(size_t i=0, size=activatables.size(); i<size; ++i)
	{
		// TODO: Determine whether the activatable is in a visible leaf and skip further testing if so.

		ICmpAABBBounds_Ptr cmpBounds = objectManager->get_component(activatables[i], cmpBounds);
		ICmpPosition_Ptr cmpPosition = objectManager->get_component(activatables[i], cmpPosition);

		const AABB3d& aabb = objectManager->aabbs()[cmpBounds->cur_aabb_index()];
		const Vector3d& position = cmpPosition->position();
		AABB3d tAABB = aabb.translate(position);
		boost::optional<std::pair<Vector3d,Vector3d> > result = determine_halfray_intersection_with_aabb(eye, look, tAABB);
		if(result)
		{
			Vector3d hit = result->first;
			double distSquared = eye.distance_squared(hit);
			if(distSquared < nearestDistSquared)
			{
				nearestObject = activatables[i];
				nearestDistSquared = distSquared;
			}
		}
	}

	const double RANGE = 3.0;
	if(!nearestObject.valid() || nearestDistSquared > RANGE*RANGE) return;

	// Step 4:	If the player is trying to activate an object, activate the nearest object. Otherwise, set it to be highlighted.
	if(input.mouse_button_down(UserInput::BUTTON_RIGHT))
	{
		ICmpActivatable_Ptr cmpActivatable = objectManager->get_component(nearestObject, cmpActivatable);
		cmpActivatable->activated_by(objectManager->player());
		input.release_mouse_button(UserInput::BUTTON_RIGHT);
	}
	else
	{
		ICmpModelRender_Ptr cmpRender = objectManager->get_component(nearestObject, cmpRender);
		cmpRender->set_highlights(true);
	}
}

void GameState_Level::do_animations(int milliseconds)
{
	// Update the model animations.
	const ObjectManager_Ptr& objectManager = m_level->object_manager();
	std::vector<ObjectID> animatables = objectManager->group("Animatables");
	for(size_t i=0, size=animatables.size(); i<size; ++i)
	{
		ICmpModelRender_Ptr cmpRender = objectManager->get_component(animatables[i], cmpRender);
		cmpRender->anim_controller()->update(milliseconds);
	}
}

void GameState_Level::do_physics(int milliseconds)
{
	// Apply gravity to simulable objects (i.e. those which have a physics component).

	// FIXME: Gravity strength should eventually be a level property.
	const double GRAVITY_STRENGTH = 9.81;	// strength of gravity in Newtons

	const ObjectManager_Ptr& objectManager = m_level->object_manager();
	std::vector<ObjectID> simulables = objectManager->group("Simulables");
	for(size_t i=0, size=simulables.size(); i<size; ++i)
	{
		ICmpPhysics_Ptr cmpPhysics = objectManager->get_component(simulables[i], cmpPhysics);
		Vector3d velocity = cmpPhysics->velocity();
		cmpPhysics->set_velocity(velocity + Vector3d(0,0,-GRAVITY_STRENGTH*(milliseconds/1000.0)));
		if(MoveFunctions::single_move_without_navmesh(simulables[i], objectManager, cmpPhysics->velocity(), 7.0 /* FIXME */, m_level->onion_tree(), milliseconds))
		{
			// A collision occurred, so set the velocity back to zero.
			cmpPhysics->set_velocity(Vector3d(0,0,0));
		}
	}
}

void GameState_Level::do_yokes(int milliseconds, UserInput& input)
{
	// Step 1:	Generate the desired object commands for the yokeable objects and add them to the queue.
	const ObjectManager_Ptr& objectManager = m_level->object_manager();
	std::vector<ObjectID> yokeables = objectManager->group("Yokeables");

	std::list<ObjectCommand_Ptr> cmdQueue;	// implement the queue as a list so that we can use back_inserter below

	for(size_t i=0, size=yokeables.size(); i<size; ++i)
	{
		ICmpYoke_Ptr cmpYoke = objectManager->get_component(yokeables[i], cmpYoke);
		
		// Use the object's yoke component to generate object commands.
		std::vector<ObjectCommand_Ptr> commands = cmpYoke->generate_commands(input, m_level->onion_polygons(), m_level->onion_tree(), m_level->nav_datasets());

		// Append the object commands to the queue.
		std::copy(commands.begin(), commands.end(), std::back_inserter(cmdQueue));
	}

	// Step 2:	Execute the object commands.
	for(std::list<ObjectCommand_Ptr>::const_iterator it=cmdQueue.begin(), iend=cmdQueue.end(); it!=iend; ++it)
	{
		(*it)->execute(objectManager, m_level->onion_polygons(), m_level->onion_tree(), m_level->nav_datasets(), milliseconds);
	}
}

void GameState_Level::grab_input()
{
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	m_inputGrabbed = true;
}

void GameState_Level::ungrab_input()
{
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	SDL_ShowCursor(SDL_ENABLE);
	m_inputGrabbed = false;
}

}
