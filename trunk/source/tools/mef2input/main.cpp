/***
 * mef2input: main.cpp
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
	std::cout << "Usage: mef2input <input MEF> <output brushes> <output entities> <output lights>" << std::endl;
	exit(EXIT_FAILURE);
}

void run_converter(const std::string& inputFilename, const std::string& brushesFilename, const std::string& entitiesFilename, const std::string& lightsFilename)
{
	// TODO
}

int main(int argc, char *argv[])
{
	if(argc != 5) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_converter(args[1], args[2], args[3], args[4]);
	return 0;
}
