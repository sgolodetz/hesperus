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
bool UserInput::key_down(SDLKey key) const
{
	return m_keyDown[key];
}

int UserInput::mouse_motion_x() const
{
	return m_mouseMotionX;
}

int UserInput::mouse_motion_y() const
{
	return m_mouseMotionY;
}

void UserInput::press_key(SDLKey key)
{
	m_keyDown[key] = true;
}

void UserInput::release_key(SDLKey key)
{
	m_keyDown[key] = false;
}

void UserInput::reset()
{
	m_keyDown = boost::dynamic_bitset<>(SDLK_LAST, false);
	set_mouse_motion(0, 0);
}

void UserInput::set_mouse_motion(int x, int y)
{
	m_mouseMotionX = x;
	m_mouseMotionY = y;
}

}
