/***
 * hesperus: EntDefFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTDEFFILE
#define H_HESP_ENTDEFFILE

#include <map>
#include <string>
#include <vector>

#include <source/math/geom/AABB.h>

namespace hesp {

struct EntDefFile
{
	//#################### LOADING METHODS ####################
	static std::vector<AABB3d> load_aabbs_only(const std::string& filename);
};

}

#endif
