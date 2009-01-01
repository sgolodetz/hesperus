/***
 * hcollate: main.cpp
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
	std::cout << "Usage: hcollate <input lit tree> <input portals> <input vis> <output filename>" << std::endl;
	exit(EXIT_FAILURE);
}

void collate(const std::string& treeFilename, const std::string& portalsFilename, const std::string& visFilename, const std::string& outputFilename)
{
	

	// TODO
}

int main(int argc, char *argv[])
{
	if(argc != 5) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	collate(args[0], args[1], args[2], args[3]);
	return 0;
}
