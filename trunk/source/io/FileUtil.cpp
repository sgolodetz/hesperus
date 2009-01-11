/***
 * hesperus: FileUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FileUtil.h"

#include <fstream>

#include <source/exceptions/Exception.h>

namespace hesp {

//#################### PRIVATE METHODS ####################
/**
Loads a separator (a line containing only ***) from the specified std::istream.

@param is	The std::istream
*/
void FileUtil::load_separator(std::istream& is)
{
	std::string line;
	if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read separator");
	if(line != "***") throw Exception("Bad separator");
}

}
