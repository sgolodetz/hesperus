/***
 * hesperus: Game.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_GAME
#define H_HESP_GAME

#include <iostream>
#include <string>

#include <SDL.h>

#include <source/util/UserInput.h>
#include "GameState.h"

namespace hesp {

class Game
{
	//#################### PRIVATE VARIABLES ####################
private:
	GameState_Ptr m_state;
	UserInput m_input;

	//#################### CONSTRUCTORS ####################
public:
	Game();

	//#################### PUBLIC METHODS ####################
public:
	void run();

	//#################### PRIVATE METHODS ####################
private:
	void handle_key_down(const SDL_keysym& keysym);
	void handle_key_up(const SDL_keysym& keysym);
	void handle_mousebutton_down(const SDL_MouseButtonEvent& e);
	void handle_mousebutton_up(const SDL_MouseButtonEvent& e);
	void process_events();
	void quit(int code);
	void quit_with_error(const std::string& error);
};

}

#endif
