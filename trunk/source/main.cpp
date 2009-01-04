/***
 * hesperus: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <SDL.h>
#include <gl/glew.h>
#include <source/ogl/WrappedGL.h>
#include <gl/glu.h>

#include <source/gui/ExplicitLayout.h>
#include <source/gui/Picture.h>
#include <source/gui/Screen.h>
#include <source/io/FileUtil.h>
#include <source/level/LevelViewer.h>
using namespace hesp;

void quit(int code)
{
	SDL_Quit();
	exit(code);
}

void quit_with_error(const std::string& error)
{
	SDL_Quit();
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
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

int main(int argc, char* argv[])
try
{
	Level_Ptr level = FileUtil::load_level_file("resources/simpleg.bsp");

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

	if(glewInit() != GLEW_OK) quit_with_error("GLEW could not be initialised");
	if(!glewGetExtension("GL_ARB_multitexture")) quit_with_error("Multitexturing not supported");

	// Setup the screen.
	Screen& screen = Screen::instance();
	screen.layout().add(new Picture("resources/title.bmp"), Extents(width/4, 0, width*3/4, width/8));
	screen.layout().add(new LevelViewer(level), Extents(50, 200, width - 50, height - 50));
	Container<ExplicitLayout> *cont = new Container<ExplicitLayout>;
	cont->layout().add(new Picture("resources/title.bmp"), Extents(500, 0, 700, 50));
	screen.layout().add(cont, Extents(100, 100, 200, 200));
	screen.fit(Extents(0, 0, 1024, 768));

	for(;;)
	{
		process_events();
		screen.render();
	}

	return 0;
}
catch(Exception& e)
{
	quit_with_error(e.cause());
	return EXIT_FAILURE;	// to keep the compiler happy (this will never actually happen)
}
