/***
 * hesperus: UserInput.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "UserInput.h"

#include <source/exceptions/Exception.h>

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

bool UserInput::mouse_position_known() const
{
	return m_mousePositionX != -1;
}

int UserInput::mouse_position_x() const
{
	if(m_mousePositionX != -1) return m_mousePositionX;
	else throw Exception("The mouse position is not yet known");
}

int UserInput::mouse_position_y() const
{
	if(m_mousePositionY != -1) return m_mousePositionY;
	else throw Exception("The mouse position is not yet known");
}

int UserInput::mouse_pressed_x(MouseButton button) const
{
	if(m_mousePressedX[button] != -1) return m_mousePressedX[button];
	else throw Exception("The specified mouse button is not currently pressed");
}

int UserInput::mouse_pressed_y(MouseButton button) const
{
	if(m_mousePressedY[button] != -1) return m_mousePressedY[button];
	else throw Exception("The specified mouse button is not currently pressed");
}

void UserInput::press_key(SDLKey key)
{
	m_keyDown[key] = true;
}

void UserInput::press_mouse_button(MouseButton button, int x, int y)
{
	m_mouseButtonDown[button] = true;
	m_mousePressedX[button] = x;
	m_mousePressedY[button] = y;
}

void UserInput::release_key(SDLKey key)
{
	m_keyDown[key] = false;
}

void UserInput::release_mouse_button(MouseButton button)
{
	m_mouseButtonDown[button] = false;
	m_mousePressedX[button] = m_mousePressedY[button] = -1;
}

void UserInput::reset()
{
	m_keyDown = boost::dynamic_bitset<>(SDLK_LAST, false);
	m_mouseButtonDown = boost::dynamic_bitset<>(BUTTON_LAST, false);
	set_mouse_motion(0, 0);
	set_mouse_position(-1, -1);
	m_mousePressedX = m_mousePressedY = std::vector<int>(BUTTON_LAST, -1);
}

void UserInput::set_mouse_motion(int x, int y)
{
	m_mouseMotionX = x;
	m_mouseMotionY = y;
}

void UserInput::set_mouse_position(int x, int y)
{
	m_mousePositionX = x;
	m_mousePositionY = y;
}

}
