/***
 * hesperus: UserInput.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_USERINPUT
#define H_HESP_USERINPUT

#include <boost/dynamic_bitset.hpp>

#include <sdl_keysym.h>

namespace hesp {

class UserInput
{
	//#################### PRIVATE VARIABLES ####################
private:
	boost::dynamic_bitset<> m_keyDown;

	//#################### CONSTRUCTORS ####################
public:
	UserInput();

	//#################### PUBLIC METHODS ####################
public:
	bool key_down(SDLKey key) const;
	void press_key(SDLKey key);
	void release_key(SDLKey key);
	void reset();
};

}

#endif
