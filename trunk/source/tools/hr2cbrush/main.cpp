/***
 * hr2cbrush: main.cpp
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
	std::cout << "Usage: hr2cbrush <input filename> <output filename>" << std::endl;
	exit(EXIT_FAILURE);
}

void run_converter(const std::string& inputFilename, const std::string& outputFilename)
{
	// TODO
}

int main(int argc, char *argv[])
{
	if(argc != 3) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_converter(args[1], args[2]);
	return 0;
}
