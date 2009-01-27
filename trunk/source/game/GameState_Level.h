/***
 * hesperus: GameState_Level.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
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

	//#################### CONSTRUCTORS ####################
public:
	GameState_Level(const std::string& levelFilename);

	//#################### PUBLIC METHODS ####################
public:
	GameState_Ptr update(int milliseconds, const UserInput& input);

	//#################### PRIVATE METHODS ####################
private:
	Component_Ptr construct_display();
};

}

#endif
