/***
 * hesperus: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cstdlib>
#include <iostream>

#include <source/exceptions/Exception.h>
#include <source/game/Game.h>
#if 1
#include <source/io/ModelFilesUtil.h>
#endif
using namespace hesp;

int main(int argc, char* argv[])
try
{
#if 0
	// FIXME: Remove this testing code (and the include above).
	Skeleton_Ptr skeleton = ModelFilesUtil::load_skeleton("resources/models/Test.skeleton.xml");
#endif

	Game game;
	game.run();

	return 0;
}
catch(Exception& e)
{
	std::cerr << e.cause() << std::endl;
	return EXIT_FAILURE;
}
catch(...)
{
	std::cerr << "Error: Something strange and unexpected happened" << std::endl;
	return EXIT_FAILURE;
}
