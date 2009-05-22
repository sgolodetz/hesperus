/***
 * hesperus: GameState_Level.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "GameState_Level.h"

#include <algorithm>
#include <iostream>
#include <list>

#include <SDL.h>

#include <source/gui/Container.h>
#include <source/gui/ExplicitLayout.h>
#include <source/gui/Picture.h>
#include <source/gui/Screen.h>
#include <source/io/files/LevelFile.h>
#include <source/io/util/DirectoryFinder.h>
#include <source/level/LevelViewer.h>
#include <source/level/objects/ICmpPhysics.h>
#include <source/level/objects/ICmpRender.h>
#include <source/level/objects/ICmpYoke.h>
#include <source/level/objects/MoveFunctions.h>
#include <source/level/objects/ObjectCommand.h>
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

	do_objects(milliseconds, input);
	do_physics(milliseconds);
	do_animations(milliseconds);

	return GameState_Ptr();
}

//#################### PRIVATE METHODS ####################
Component_Ptr GameState_Level::construct_display()
{
	Container<ExplicitLayout> *display = new Container<ExplicitLayout>;

	bf::path imagesDir = determine_images_directory(determine_base_directory_from_game());

	const Screen& screen = Screen::instance();
	int width = screen.dimensions().width();
	int height = screen.dimensions().height();
	display->layout().add(new Picture((imagesDir / "title.png").file_string()), Extents(width/4, 0, width*3/4, width/8));
	display->layout().add(new LevelViewer(m_level), Extents(50, 200, width - 50, height - 50));
	Container<ExplicitLayout> *cont = new Container<ExplicitLayout>;
	cont->layout().add(new Picture((imagesDir / "title.png").file_string()), Extents(500, 0, 700, 50));
	display->layout().add(cont, Extents(100, 100, 200, 200));

	return Component_Ptr(display);
}

void GameState_Level::do_animations(int milliseconds)
{
	// Update the model animations.
	const ObjectManager_Ptr& objectManager = m_level->object_manager();
	std::vector<ObjectID> animatables = objectManager->group("Animatables");
	for(size_t i=0, size=animatables.size(); i<size; ++i)
	{
		ICmpRender_Ptr cmpRender = objectManager->get_component(animatables[i], cmpRender);
		cmpRender->anim_controller()->update(milliseconds);
	}
}

void GameState_Level::do_objects(int milliseconds, UserInput& input)
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
