/***
 * hesperus: EntitiesFileUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITIESFILEUTIL
#define H_HESP_ENTITIESFILEUTIL

#include <string>

#include <boost/filesystem/operations.hpp>

#include <source/level/entities/EntityExManager.h>

namespace hesp {

struct EntitiesFileUtil
{
	//#################### LOADING METHODS ####################
	static EntityManager_Ptr load(const std::string& filename, const boost::filesystem::path& settingsDir);
};

}

#endif
