/***
 * hesperus: EntDefFileUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntDefFileUtil.h"

#include <fstream>

#include <source/exceptions/Exception.h>
#include <source/io/sections/AABBsSection.h>
#include <source/io/sections/EntityComponentsSection.h>
#include <source/io/sections/EntityTypesSection.h>
#include <source/io/util/LineIO.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads an array of AABBs and an entity components map from the specified entity definition file.

@param filename	The name of the file from which to load
@return			The AABBs and entity components map
*/
void EntDefFileUtil::load(const std::string& filename, std::vector<AABB3d>& aabbs,
						  std::map<std::string,EntityComponents>& entityComponentsMap)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	LineIO::read_checked_line(is, "HENTDEF");
	aabbs = AABBsSection::load(is);
	EntityComponentsSection::load(is);	// the game itself doesn't use anything from this section
	entityComponentsMap = EntityTypesSection::load(is);
}

/**
Loads an array of AABBs from the specified entity definition file.

@param filename	The name of the file from which to load the AABBs
@return			The AABBs
*/
std::vector<AABB3d> EntDefFileUtil::load_aabbs_only(const std::string& filename)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	LineIO::read_checked_line(is, "HENTDEF");
	return AABBsSection::load(is);
}

}
