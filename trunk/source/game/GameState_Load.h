/***
 * hesperus: GameState_Load.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_GAMESTATE_LOAD
#define H_HESP_GAMESTATE_LOAD

#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
using boost::shared_ptr;

#include "GameState.h"

namespace hesp {

class GameState_Load : public GameState
{
	//#################### PRIVATE VARIABLES ####################
private:
	boost::mutex m_doneMutex;
	bool m_done;
	shared_ptr<boost::thread> m_loadThread;
	GameState_Ptr m_levelState;

	//#################### CONSTRUCTORS ####################
public:
	GameState_Load(const std::string& levelFilename);

	//#################### PUBLIC METHODS ####################
public:
	void enter();
	void leave();
	GameState_Ptr update(int milliseconds, UserInput& input);

	//#################### PRIVATE METHODS ####################
private:
	Component_Ptr construct_display();
	void load(const std::string& levelFilename);
};

}

#endif
