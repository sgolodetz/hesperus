/***
 * hesperus: Game.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Game.h"

#include <gl/glew.h>
#include <source/ogl/WrappedGL.h>
#include <gl/glu.h>

#include <source/exceptions/Exception.h>
#include <source/gui/Screen.h>
#include "GameState_Level.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Game::Game()
try
{
	// Set up the window.
	if(SDL_Init(SDL_INIT_VIDEO) < 0) quit(EXIT_FAILURE);

	SDL_WM_SetCaption("The Scarlet Pimpernel", NULL);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	int width = 1024;
	int height = 768;
	int bpp = 32;
	int flags = SDL_OPENGL;// | SDL_FULLSCREEN;

	if(SDL_SetVideoMode(width, height, bpp, flags) == 0) quit(EXIT_FAILURE);

	Screen& screen = Screen::instance();
	screen.set_dimensions(Extents(0, 0, width, height));
	screen.fit();

	if(glewInit() != GLEW_OK) quit_with_error("GLEW could not be initialised");
	if(!glewGetExtension("GL_ARB_multitexture")) quit_with_error("Multitexturing not supported");

	// Set the initial game state.
	m_state.reset(new GameState_Level("resources/levels/tricky/tricky.bsp"));
}
catch(Exception& e) { quit_with_error(e.cause()); }

//#################### PUBLIC METHODS ####################
void Game::run()
{
	const Screen& screen = Screen::instance();

	Uint32 lastUpdate = SDL_GetTicks();
	Uint32 lastDraw = SDL_GetTicks();
	for(;;)
	{
		process_events();

		Uint32 frameTime = SDL_GetTicks();

		// TODO: Pass in the user input as a parameter to the update method.
#if 0
		std::cout << "Update " << frameTime - lastUpdate << std::endl;
#endif
		m_state->update(frameTime - lastUpdate);
		lastUpdate = frameTime;

		if(frameTime - lastDraw >= 20)	// aim for 50 frames per second
		{
#if 0
			std::cout << "Render " << frameTime - lastDraw << std::endl;
#endif
			screen.render();
			lastDraw = frameTime;
		}
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
