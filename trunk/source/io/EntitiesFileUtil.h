/***
 * hesperus: EntitiesFileUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITIESFILEUTIL
#define H_HESP_ENTITIESFILEUTIL

#include <string>

#include <source/level/entities/EntityManager.h>

namespace hesp {

struct EntitiesFileUtil
{
	//#################### LOADING METHODS ####################
	static EntityManager_Ptr load(const std::string& filename);
};

}

#endif