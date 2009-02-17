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
#include <source/io/LevelFileUtil.h>
#include <source/level/entities/MovementFunctions.h>
#include <source/level/LevelViewer.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
GameState_Level::GameState_Level(const std::string& levelFilename)
{
	m_level = LevelFileUtil::load(levelFilename);
}

//#################### PUBLIC METHODS ####################
void GameState_Level::enter()
{
	// Clear all pending SDL events before we get started.
	{ SDL_Event e; while(SDL_PollEvent(&e)) {} }

	set_display(construct_display());
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_GrabInput(SDL_GRAB_ON);
}

void GameState_Level::leave()
{
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	SDL_ShowCursor(SDL_ENABLE);
}

GameState_Ptr GameState_Level::update(int milliseconds, UserInput& input)
{
	// TEMPORARY: Allow quick toggling of the input grab using the 'g' key (for debugging purposes).
	static bool inputGrabbed = true;
	if(input.key_down(SDLK_g))
	{
		input.release_key(SDLK_g);
		if(inputGrabbed)
		{
			SDL_WM_GrabInput(SDL_GRAB_OFF);
			SDL_ShowCursor(SDL_ENABLE);
		}
		else
		{
				SDL_ShowCursor(SDL_DISABLE);
				SDL_WM_GrabInput(SDL_GRAB_ON);
		}
		inputGrabbed = !inputGrabbed;
	}

	// Step 1:	Generate the desired entity commands for the yokeable entities and add them to the queue.
	const EntityManager_Ptr& entityManager = m_level->entity_manager();
	const std::vector<Entity_Ptr>& yokeables = entityManager->yokeables();

	std::list<EntityCommand_Ptr> cmdQueue;	// implement the queue as a list so that we can use back_inserter below

	int yokeableCount = static_cast<int>(yokeables.size());
	for(int i=0; i<yokeableCount; ++i)
	{
		const Yoke_Ptr& yoke = yokeables[i]->yoke_component()->yoke();
		if(yoke)
		{
			// Use the entity's yoke to generate entity commands.
			std::vector<EntityCommand_Ptr> commands = yoke->generate_commands(input, m_level->nav_datasets());

			// Append the entity commands to the queue.
			std::copy(commands.begin(), commands.end(), std::back_inserter(cmdQueue));
		}
	}

	// Step 2:	Execute the entity commands.
	for(std::list<EntityCommand_Ptr>::const_iterator it=cmdQueue.begin(), iend=cmdQueue.end(); it!=iend; ++it)
	{
		(*it)->execute(m_level->entity_manager()->aabbs(), m_level->onion_polygons(), m_level->onion_tree(), m_level->nav_datasets(), milliseconds);
	}

	// Step 3:	Apply gravity to simulable entities (i.e. those which have a physics component).
	// FIXME: Gravity strength should eventually be a level property.
	const double GRAVITY_STRENGTH = 9.81;	// strength of gravity in Newtons

	const std::vector<Entity_Ptr> simulables = entityManager->simulables();
	int simulableCount = static_cast<int>(simulables.size());
	for(int i=0; i<simulableCount; ++i)
	{
		IPhysicsComponent_Ptr physComponent = simulables[i]->physics_component();
		Vector3d velocity = physComponent->velocity();
		physComponent->set_velocity(velocity + Vector3d(0,0,-GRAVITY_STRENGTH*(milliseconds/1000.0)));
		if(MovementFunctions::single_move_without_navmesh(simulables[i], physComponent->velocity(), m_level->onion_tree(), milliseconds))
		{
			// A collision occurred, so set the velocity back to zero.
			physComponent->set_velocity(Vector3d(0,0,0));
		}
	}

	return GameState_Ptr();
}

//#################### PRIVATE METHODS ####################
Component_Ptr GameState_Level::construct_display()
{
	Container<ExplicitLayout> *display = new Container<ExplicitLayout>;

	const Screen& screen = Screen::instance();
	int width = screen.dimensions().width();
	int height = screen.dimensions().height();
	display->layout().add(new Picture("resources/images/title.bmp"), Extents(width/4, 0, width*3/4, width/8));
	display->layout().add(new LevelViewer(m_level), Extents(50, 200, width - 50, height - 50));
	Container<ExplicitLayout> *cont = new Container<ExplicitLayout>;
	cont->layout().add(new Picture("resources/images/title.bmp"), Extents(500, 0, 700, 50));
	display->layout().add(cont, Extents(100, 100, 200, 200));

	return Component_Ptr(display);
}

}
