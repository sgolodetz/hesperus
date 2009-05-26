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

bool UserInput::mouse_button_down(MouseButton button) const
{
	return m_mouseButtonDown[button];
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

void UserInput::press_mouse_button(MouseButton button)
{
	m_mouseButtonDown[button] = true;
}

void UserInput::release_key(SDLKey key)
{
	m_keyDown[key] = false;
}

void UserInput::release_mouse_button(MouseButton button)
{
	m_mouseButtonDown[button] = false;
}

void UserInput::reset()
{
	m_keyDown = boost::dynamic_bitset<>(SDLK_LAST, false);
	m_mouseButtonDown = boost::dynamic_bitset<>(BUTTON_LAST, false);
	set_mouse_motion(0, 0);
}

void UserInput::set_mouse_motion(int x, int y)
{
	m_mouseMotionX = x;
	m_mouseMotionY = y;
}

}
