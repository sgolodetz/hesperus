/***
 * hbsp: QuitFunctions.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "QuitFunctions.h"

namespace hesp {

void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: hbsp {-r|-c} <input filename> <output filename> [-w<number>]" << std::endl;
	exit(EXIT_FAILURE);
}

}
