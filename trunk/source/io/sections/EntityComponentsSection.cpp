/***
 * hesperus: EntityComponentsSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntityComponentsSection.h"

#include <source/io/util/LineIO.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads an EntityComponents section from a std::istream. These are present in entity definition files.

@param is	The std::istream
*/
void EntityComponentsSection::load(std::istream& is)
{
	// Note: This function skips over the EntityComponents section because it's only relevant to the map editor.

	LineIO::read_checked_line(is, "EntityComponents");
	LineIO::read_checked_line(is, "{");

	std::string line;
	int bracketCount = 1;
	while(bracketCount > 0)
	{
		LineIO::read_line(is, line, "entity components section");
		if(line == "{") ++bracketCount;
		if(line == "}") --bracketCount;
	}
}

}
