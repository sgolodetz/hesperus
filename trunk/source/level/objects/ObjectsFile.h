/***
 * hesperus: ObjectsFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTSFILE
#define H_HESP_OBJECTSFILE

#include <boost/filesystem/operations.hpp>

#include "ObjectManager.h"

namespace hesp {

struct ObjectsFile
{
	//#################### LOADING METHODS ####################
	static ObjectManager_Ptr load(const std::string& filename, const std::vector<AABB3d>& aabbs, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes, const boost::filesystem::path& baseDir);

	//#################### SAVING METHODS ####################
	static void save(const std::string& filename, const ObjectManager_Ptr& objectManager);
};

}

#endif
