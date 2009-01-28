/***
 * hesperus: GameState_Level.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "GameState_Level.h"

#include <algorithm>
#include <list>

#include <source/gui/Container.h>
#include <source/gui/ExplicitLayout.h>
#include <source/gui/Picture.h>
#include <source/gui/Screen.h>
#include <source/io/LevelFileUtil.h>
#include <source/level/LevelViewer.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
GameState_Level::GameState_Level(const std::string& levelFilename)
{
	m_level = LevelFileUtil::load(levelFilename);
	set_display(construct_display());
}

//#################### PUBLIC METHODS ####################
GameState_Ptr GameState_Level::update(int milliseconds, const UserInput& input)
{
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
			std::vector<EntityCommand_Ptr> commands = yoke->generate_commands(input);

			// Append the entity commands to the queue.
			std::copy(commands.begin(), commands.end(), std::back_inserter(cmdQueue));
		}
	}

	// Step 2:	Execute the entity commands.
	for(std::list<EntityCommand_Ptr>::const_iterator it=cmdQueue.begin(), iend=cmdQueue.end(); it!=iend; ++it)
	{
		(*it)->execute(milliseconds);
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
	display->layout().add(new Picture("resources/title.bmp"), Extents(width/4, 0, width*3/4, width/8));
	display->layout().add(new LevelViewer(m_level), Extents(50, 200, width - 50, height - 50));
	Container<ExplicitLayout> *cont = new Container<ExplicitLayout>;
	cont->layout().add(new Picture("resources/title.bmp"), Extents(500, 0, 700, 50));
	display->layout().add(cont, Extents(100, 100, 200, 200));

	return Component_Ptr(display);
}

}
