/***
 * hesperus: UserInput.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "UserInput.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
UserInput::UserInput()
{
	reset();
}

//#################### PUBLIC METHODS ####################
void UserInput::key_down(SDLKey key)
{
	m_keyDown[key] = true;
}

void UserInput::key_up(SDLKey key)
{
	m_keyDown[key] = false;
}

void UserInput::reset()
{
	m_keyDown = boost::dynamic_bitset<>(SDLK_LAST, false);
}

}
