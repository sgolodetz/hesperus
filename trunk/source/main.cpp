/***
 * hesperus: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cstdlib>
#include <iostream>

#include <source/exceptions/Exception.h>
#include <source/game/Game.h>
#include <source/io/util/DirectoryFinder.h>
using namespace hesp;

namespace hesp {

boost::filesystem::path determine_base_directory()
{
	return determine_base_directory_from_game();
}

}

int main(int argc, char* argv[])
try
{
	Game game;
	game.run();

	return 0;
}
catch(Exception& e)
{
	std::cerr << "Error: " << e.cause() << std::endl;
	return EXIT_FAILURE;
}
catch(...)
{
	std::cerr << "Error: Something strange and unexpected happened" << std::endl;
	return EXIT_FAILURE;
}
