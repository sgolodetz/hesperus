/***
 * hesperus: EntDefFileUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTDEFFILEUTIL
#define H_HESP_ENTDEFFILEUTIL

#include <string>
#include <vector>

#include <source/math/geom/AABB.h>

namespace hesp {

struct EntDefFileUtil
{
	//#################### LOADING METHODS ####################
	static std::vector<AABB3d> load_aabbs_only(const std::string& filename);
};

}

#endif
