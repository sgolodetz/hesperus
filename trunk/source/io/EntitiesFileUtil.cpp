/***
 * hesperus: EntitiesFileUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntitiesFileUtil.h"

#include <fstream>

#include <source/exceptions/Exception.h>
#include "FileSectionUtil.h"

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads a set of entities from the specified entities file.

@param filename	The name of the file from which to load the entities
@return			An EntityManager containing the loaded entities
*/
EntityManager_Ptr EntitiesFileUtil::load(const std::string& filename)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");
	return FileSectionUtil::load_entities_section(is);
}

}
