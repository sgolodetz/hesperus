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
#include <source/level/entities/MovementFunctions.h>
#include <source/level/LevelViewer.h>
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

	do_entities(milliseconds, input);
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
	const EntityManager_Ptr& entityManager = m_level->entity_manager();
	const std::vector<Entity_Ptr>& animatables = entityManager->group("Animatables");
	int animatableCount = static_cast<int>(animatables.size());
	for(int i=0; i<animatableCount; ++i)
	{
		animatables[i]->character_anim_controller()->update(milliseconds);
	}
}

void GameState_Level::do_entities(int milliseconds, UserInput& input)
{
	// Step 1:	Generate the desired entity commands for the yokeable entities and add them to the queue.
	const EntityManager_Ptr& entityManager = m_level->entity_manager();
	const std::vector<Entity_Ptr>& yokeables = entityManager->group("Yokeables");

	std::list<EntityCommand_Ptr> cmdQueue;	// implement the queue as a list so that we can use back_inserter below

	int yokeableCount = static_cast<int>(yokeables.size());
	for(int i=0; i<yokeableCount; ++i)
	{
		const Yoke_Ptr& yoke = yokeables[i]->yoke();
		if(yoke)
		{
			// Use the entity's yoke to generate entity commands.
			std::vector<EntityCommand_Ptr> commands = yoke->generate_commands(input, m_level->onion_polygons(), m_level->onion_tree(), m_level->nav_datasets());

			// Append the entity commands to the queue.
			std::copy(commands.begin(), commands.end(), std::back_inserter(cmdQueue));
		}
	}

	// Step 2:	Execute the entity commands.
	for(std::list<EntityCommand_Ptr>::const_iterator it=cmdQueue.begin(), iend=cmdQueue.end(); it!=iend; ++it)
	{
		(*it)->execute(entityManager->aabbs(), m_level->onion_polygons(), m_level->onion_tree(), m_level->nav_datasets(), milliseconds);
	}
}

void GameState_Level::do_physics(int milliseconds)
{
	// Apply gravity to simulable entities (i.e. those which have a physics component).

	// FIXME: Gravity strength should eventually be a level property.
	const double GRAVITY_STRENGTH = 9.81;	// strength of gravity in Newtons

	const EntityManager_Ptr& entityManager = m_level->entity_manager();
	const std::vector<Entity_Ptr>& simulables = entityManager->group("Simulables");
	int simulableCount = static_cast<int>(simulables.size());
	for(int i=0; i<simulableCount; ++i)
	{
		Vector3d velocity = simulables[i]->velocity();
		simulables[i]->set_velocity(velocity + Vector3d(0,0,-GRAVITY_STRENGTH*(milliseconds/1000.0)));
		if(MovementFunctions::single_move_without_navmesh(simulables[i], simulables[i]->velocity(), 7.0 /* FIXME */, m_level->onion_tree(), milliseconds))
		{
			// A collision occurred, so set the velocity back to zero.
			simulables[i]->set_velocity(Vector3d(0,0,0));
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
