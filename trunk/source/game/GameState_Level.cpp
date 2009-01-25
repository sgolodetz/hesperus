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
