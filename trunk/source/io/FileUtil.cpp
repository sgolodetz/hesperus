/***
 * hesperus: FileUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FileUtil.h"

#include <fstream>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Loads an empty leaf count and an array of portals from the specified file.

@param filename			The name of the portals file
@param emptyLeafCount	Used to return the empty leaf count to the caller
@param portals			Used to return the portals to the caller
*/
void FileUtil::load_portals_file(const std::string& filename, int& emptyLeafCount, std::vector<Portal_Ptr>& portals)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("The portals file could not be read");

	std::string line;
	if(!std::getline(is, line)) throw Exception("The empty leaf count could not be read");
	try							{ emptyLeafCount = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The empty leaf count was not an integer"); }

	FileSectionUtil::load_polygons_section(is, "Portals", portals);
}

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
