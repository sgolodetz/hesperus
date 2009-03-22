/***
 * hesperus: EntityComponentsSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITYCOMPONENTSSECTION
#define H_HESP_ENTITYCOMPONENTSSECTION

#include <iosfwd>

namespace hesp {

struct EntityComponentsSection
{
	//#################### LOADING METHODS ####################
	static void load(std::istream& is);
};

}

#endif
