/***
 * hesperus: GameState.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "GameState.h"

#include <source/gui/Screen.h>

namespace hesp {

//#################### DESTRUCTOR ####################
GameState::~GameState() {}

//#################### PROTECTED METHODS ####################
void GameState::set_display(const GUIComponent_Ptr& display)
{
	Screen& screen = Screen::instance();
	screen.layout().clear();
	screen.layout().add(display, screen.dimensions());
	screen.fit();
}

}
