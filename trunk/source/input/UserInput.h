/***
 * hesperus: UserInput.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_USERINPUT
#define H_HESP_USERINPUT

#include <boost/dynamic_bitset.hpp>

#include <sdl_keysym.h>
#include <sdl_mouse.h>

namespace hesp {

class UserInput
{
	//#################### ENUMERATIONS ####################
public:
	enum MouseButton
	{
		BUTTON_LEFT = SDL_BUTTON_LEFT,
		BUTTON_MIDDLE = SDL_BUTTON_MIDDLE,
		BUTTON_RIGHT = SDL_BUTTON_RIGHT,
		BUTTON_LAST
	};

	//#################### PRIVATE VARIABLES ####################
private:
	boost::dynamic_bitset<> m_keyDown;
	boost::dynamic_bitset<> m_mouseButtonDown;
	int m_mouseMotionX, m_mouseMotionY;
	int m_mousePositionX, m_mousePositionY;
	std::vector<int> m_mousePressedX, m_mousePressedY;

	//#################### CONSTRUCTORS ####################
public:
	UserInput();

	//#################### PUBLIC METHODS ####################
public:
	bool key_down(SDLKey key) const;
	bool mouse_button_down(MouseButton button) const;
	int mouse_motion_x() const;
	int mouse_motion_y() const;
	bool mouse_position_known() const;
	int mouse_position_x() const;
	int mouse_position_y() const;
	int mouse_pressed_x(MouseButton button) const;
	int mouse_pressed_y(MouseButton button) const;
	void press_key(SDLKey key);
	void press_mouse_button(MouseButton button, int x, int y);
	void release_key(SDLKey key);
	void release_mouse_button(MouseButton button);
	void reset();
	void set_mouse_motion(int x, int y);
	void set_mouse_position(int x, int y);
};

}

#endif
