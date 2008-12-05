/***
 * hesperus: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cstdio>
#include <cstdlib>

#include <SDL.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <gl/gl.h>
#include <gl/glu.h>

#include "gui/ExplicitLayout.h"
#include "gui/Picture.h"
#include "gui/Screen.h"
using namespace hesp;

void quit(int code)
{
	SDL_Quit();
	exit(code);
}

void handle_key_down(SDL_keysym *keysym)
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

void process_events()
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

int main( int argc, char* argv[] )
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0) quit(EXIT_FAILURE);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	int width = 1024;
	int height = 768;
	int bpp = 32;
	int flags = SDL_OPENGL | SDL_FULLSCREEN;

	if(SDL_SetVideoMode(width, height, bpp, flags) == 0) quit(EXIT_FAILURE);

	// Setup the screen.
	Screen& screen = Screen::instance();
	screen.layout().add(new Picture, Extents(10, 10, 20, 20));
	Container<ExplicitLayout> *cont = new Container<ExplicitLayout>;
	cont->layout().add(new Picture, Extents(5, 0, 20, 10));
	screen.layout().add(cont, Extents(100, 100, 200, 200));
	screen.fit(Extents(0, 0, 1024, 768));

	for(;;)
	{
		process_events();
		screen.render();
	}

	return 0;
}
