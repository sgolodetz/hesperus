/***
 * hesperus: GameState.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_GAMESTATE
#define H_HESP_GAMESTATE

#include <source/gui/Component.h>
#include <source/util/UserInput.h>

namespace hesp {

//#################### TYPEDEFS ####################
typedef shared_ptr<class GameState> GameState_Ptr;

class GameState
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~GameState();

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual GameState_Ptr update(int milliseconds, const UserInput& input) = 0;

	//#################### PUBLIC METHODS ####################
public:
	void set_display(const Component_Ptr& display);
};

}

#endif
