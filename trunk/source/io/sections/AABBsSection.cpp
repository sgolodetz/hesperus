/***
 * hesperus: AABBsSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AABBsSection.h"

#include <source/exceptions/Exception.h>
#include <source/io/LineIO.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads an array of AABBs from the specified std::istream.

@param is			The std::istream
@return				The AABBs
@throws Exception	If EOF is encountered whilst trying to read the AABBs
*/
std::vector<AABB3d> AABBsSection::load(std::istream& is)
{
	std::vector<AABB3d> ret;

	LineIO::read_checked_line(is, "AABBs");
	LineIO::read_checked_line(is, "{");

	std::string line;
	while(std::getline(is, line))
	{
		if(line == "}") return ret;

		std::string::size_type i = line.find('=');
		if(i == std::string::npos) throw Exception("Bad AABB data encountered: missing equals");
		if(line.length() <= i+2) throw Exception("Missing AABB data after equals");
		line = line.substr(i+2);
		ret.push_back(read_aabb<Vector3d>(line));
	}

	// If we get here, we must have run out of lines to read before we saw a }.
	throw Exception("Unexpected EOF whilst trying to read the AABBs");
}

}
