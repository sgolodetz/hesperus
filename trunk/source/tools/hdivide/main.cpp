/***
 * hdivide: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

//#################### FUNCTIONS ####################
void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: hdivide <input brushes> <output normal brushes> <output collision brushes> <output detail polygons> <output hint planes> <output special brushes>" << std::endl;
	exit(EXIT_FAILURE);
}

void run_divider(const std::string& inputBrushesFilename, const std::string& normalBrushesFilename,
				 const std::string& collisionBrushesFilename, const std::string& detailPolygonsFilename,
				 const std::string& hintPlanesFilename, const std::string& specialBrushesFilename)
{
	// TODO
}

int main(int argc, char *argv[])
{
	if(argc != 7) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);

	// Validate arguments.
	// TODO

	run_divider(args[1], args[2], args[3], args[4], args[5], args[6]);

	return 0;
}
