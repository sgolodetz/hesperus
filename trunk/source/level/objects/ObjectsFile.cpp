/***
 * hesperus: ObjectsFile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ObjectsFile.h"

#include <fstream>

#include <source/exceptions/Exception.h>
#include "ObjectsSection.h"

namespace hesp {

//#################### LOADING METHODS ####################
ObjectManager_Ptr ObjectsFile::load(const std::string& filename, const std::vector<AABB3d>& aabbs,
									const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes,
									const boost::filesystem::path& baseDir)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");
	return ObjectsSection::load(is, aabbs, componentPropertyTypes, baseDir);
}

//#################### SAVING METHODS ####################
void ObjectsFile::save(const std::string& filename, const ObjectManager_Ptr& objectManager)
{
	// NYI
	throw 23;
}

}
