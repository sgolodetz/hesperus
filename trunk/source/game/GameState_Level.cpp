/***
 * hesperus: GameState_Level.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "GameState_Level.h"

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
GameState_Ptr GameState_Level::update(int milliseconds)
{
	// Step 1:	Generate the desired entity commands and add them to the queue.
	const EntityManager_Ptr& entityManager = m_level->entity_manager();
	const std::vector<BipedEntity_Ptr>& bipeds = entityManager->bipeds();

	int bipedCount = static_cast<int>(bipeds.size());
	for(int i=0; i<bipedCount; ++i)
	{
		// TODO: Use the biped's yoke to generate the entity commands
		// TODO: Append the entity commands to the queue
	}

	// Step 2:	Execute the entity commands, resolving collisions where necessary.

	// TODO

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
