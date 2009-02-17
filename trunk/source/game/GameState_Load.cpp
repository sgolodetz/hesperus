/***
 * hesperus: GameState_Load.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameState_Load.h"

#include <source/gui/Container.h>
#include <source/gui/ExplicitLayout.h>
#include <source/gui/Picture.h>
#include <source/gui/Screen.h>
#include "GameState_Level.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
GameState_Load::GameState_Load(const std::string& levelFilename)
:	m_levelFilename(levelFilename)
{}

//#################### PUBLIC METHODS ####################
void GameState_Load::enter()
{
	set_display(construct_display());
}

void GameState_Load::leave()
{
	// Nothing to do
}

GameState_Ptr GameState_Load::update(int milliseconds, UserInput& input)
{
	// Ensure that the loading screen's been rendered before we try and load the level (render happens after updating).
	static bool firstTime = true;
	if(firstTime)
	{
		firstTime = false;
		return GameState_Ptr();
	}

	return GameState_Ptr(new GameState_Level(m_levelFilename));
}

//#################### PRIVATE METHODS ####################
Component_Ptr GameState_Load::construct_display()
{
	Container<ExplicitLayout> *display = new Container<ExplicitLayout>;

	const Screen& screen = Screen::instance();
	int width = screen.dimensions().width();
	int height = screen.dimensions().height();
	display->layout().add(new Picture("resources/images/loading.bmp"), Extents(width/4, 0, width*3/4, width/8));

	// TODO: Load the appropriate image for the level here.
	display->layout().add(new Picture("resources/images/load-blakeney_hall.bmp"), Extents(50, width/8, width - 50, height - 50));

	return Component_Ptr(display);
}

}
