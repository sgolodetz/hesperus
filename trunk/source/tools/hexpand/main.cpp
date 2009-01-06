/***
 * hexpand: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <source/io/FileUtil.h>
#include <source/util/PolygonTypes.h>
using namespace hesp;

//#################### FUNCTIONS ####################
void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: hexpand <input AABBs> <input brushes> <output brushes>" << std::endl;
	exit(EXIT_FAILURE);
}

void run_expander(const std::string& aabbFilename, const std::string& inputFilename, const std::string& outputFilename)
{
	// TODO
}

int main(int argc, char *argv[])
{
	if(argc != 4) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_expander(args[1], args[2], args[3]);
	return 0;
}
