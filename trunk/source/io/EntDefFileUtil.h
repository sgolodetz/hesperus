/***
 * hesperus: EntDefFileUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTDEFFILEUTIL
#define H_HESP_ENTDEFFILEUTIL

#include <map>
#include <string>
#include <vector>

#include <source/math/geom/AABB.h>
#include "EntityComponents.h"

namespace hesp {

struct EntDefFileUtil
{
	//#################### LOADING METHODS ####################
	static void load(const std::string& filename, std::vector<AABB3d>& aabbs, std::map<std::string,EntityComponents>& entityComponentsMap);
	static std::vector<AABB3d> load_aabbs_only(const std::string& filename);
};

}

#endif
