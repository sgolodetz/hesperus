/***
 * hesperus: AABBsSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_AABBSSECTION
#define H_HESP_AABBSSECTION

#include <iosfwd>
#include <vector>

#include <source/math/geom/AABB.h>

namespace hesp {

struct AABBsSection
{
	//#################### LOADING METHODS ####################
	static std::vector<AABB3d> load(std::istream& is);
};

}

#endif
