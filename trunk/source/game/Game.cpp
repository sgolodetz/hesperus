/***
 * hesperus: Game.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Game.h"

#include <source/gui/Screen.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Game::Game()
{
}

//#################### PUBLIC METHODS ####################
void Game::run()
{
	const Screen& screen = Screen::instance();

	for(;;)
	{
		process_events();
		screen.render();
	}
}

//#################### PRIVATE METHODS ####################
void Game::handle_key_down(SDL_keysym *keysym)
{
	switch(keysym->sym)
	{
		case SDLK_ESCAPE:
			quit(0);
			break;
		default:
			break;
	}
}

void Game::process_events()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				handle_key_down(&event.key.keysym);
				break;
			case SDL_QUIT:
				quit(0);
				break;
		}
	}
}

void Game::quit(int code)
{
	SDL_Quit();
	exit(code);
}

void Game::quit_with_error(const std::string& error)
{
	SDL_Quit();
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

}
