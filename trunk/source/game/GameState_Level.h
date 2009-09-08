/***
 * hesperus: GameState_Level.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_GAMESTATE_LEVEL
#define H_HESP_GAMESTATE_LEVEL

#include <string>

#include <source/level/Level.h>
#include "GameState.h"

namespace hesp {

class GameState_Level : public GameState
{
	//#################### PRIVATE VARIABLES ####################
private:
	Level_Ptr m_level;
	bool m_inputGrabbed;

	//#################### CONSTRUCTORS ####################
public:
	explicit GameState_Level(const std::string& levelFilename);

	//#################### PUBLIC METHODS ####################
public:
	void enter();
	void leave();
	GameState_Ptr update(int milliseconds, UserInput& input);

	//#################### PRIVATE METHODS ####################
private:
	GUIComponent_Ptr construct_display();
	void do_activatables(UserInput& input);
	void do_animations(int milliseconds);
	void do_physics(int milliseconds);
	void do_yokes(int milliseconds, UserInput& input);
	void grab_input();
	void ungrab_input();
};

}

#endif
