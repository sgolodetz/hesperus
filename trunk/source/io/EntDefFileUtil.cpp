/***
 * hesperus: EntDefFileUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntDefFileUtil.h"

#include <fstream>

#include <source/exceptions/Exception.h>
#include "FileSectionUtil.h"
#include "LineIO.h"

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
	aabbs = FileSectionUtil::load_aabbs_section(is);
	FileSectionUtil::load_entity_components_section(is);	// the game itself doesn't use anything from this section
	entityComponentsMap = FileSectionUtil::load_entity_types_section(is);
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
	return FileSectionUtil::load_aabbs_section(is);
}

}
