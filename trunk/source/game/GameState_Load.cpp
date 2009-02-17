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
:	m_done(false)
{
	m_loadThread.reset(new boost::thread(&GameState_Load::load, this, levelFilename));
}

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
	boost::mutex::scoped_lock(m_doneMutex);
	if(m_done)
	{
		m_loadThread->join();
		return m_levelState;
	}
	else return GameState_Ptr();
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

void GameState_Load::load(const std::string& levelFilename)
{
	m_levelState.reset(new GameState_Level(levelFilename));

	boost::mutex::scoped_lock(m_doneMutex);
	m_done = true;
}

}
