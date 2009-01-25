/***
 * hesperus: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cstdlib>
#include <iostream>

#include <source/game/Game.h>
using namespace hesp;

int main(int argc, char* argv[])
try
{
	Game game;
	game.run();

	return 0;
}
catch(...)
{
	std::cerr << "Error: Something strange and unexpected happened" << std::endl;
	return EXIT_FAILURE;
}
