/***
 * hesperus: EntityTypesSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITYTYPESSECTION
#define H_HESP_ENTITYTYPESSECTION

#include <iosfwd>
#include <map>

#include <source/io/EntityComponents.h>

namespace hesp {

struct EntityTypesSection
{
	//#################### LOADING METHODS ####################
	static std::map<std::string,EntityComponents> load(std::istream& is);
};

}

#endif
