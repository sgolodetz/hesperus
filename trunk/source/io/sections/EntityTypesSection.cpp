/***
 * hesperus: EntityTypesSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntityTypesSection.h"

#include <boost/tokenizer.hpp>

#include <source/io/util/LineIO.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads an EntityTypes section from a std::istream. These are present in entity definition files.
*/
std::map<std::string,EntityComponents> EntityTypesSection::load(std::istream& is)
{
	// Note:	This function only loads in which components are contained in each entity type.
	//			The rest of the information is only relevant to the map editor.

	std::map<std::string,EntityComponents> entityComponentsMap;

	LineIO::read_checked_line(is, "EntityTypes");
	LineIO::read_checked_line(is, "{");

	std::string line;
	for(;;)
	{
		LineIO::read_line(is, line, "entity type");
		if(line == "}") break;

		typedef boost::char_separator<char> sep;
		typedef boost::tokenizer<sep> tokenizer;
		tokenizer tok(line.begin(), line.end(), sep(":, "));
		std::vector<std::string> tokens(tok.begin(), tok.end());

		std::string type = tokens[0];
		EntityComponents components;

		int tokenCount = static_cast<int>(tokens.size());
		for(int i=1; i<tokenCount; ++i)
		{
			components.set(tokens[i], true);
		}

		entityComponentsMap.insert(std::make_pair(type, components));

		// Skip the rest of the entity type information.
		LineIO::read_checked_line(is, "{");
		do
		{
			LineIO::read_line(is, line, "entity type");
		} while(line != "}");
	}

	return entityComponentsMap;
}

}
