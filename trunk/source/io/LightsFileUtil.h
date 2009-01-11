/***
 * hesperus: LightsFileUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IO_LIGHTSFILEUTIL
#define H_HESP_IO_LIGHTSFILEUTIL

#include <vector>

#include <source/level/lighting/Light.h>

namespace hesp {

struct LightsFileUtil
{
	//#################### LOADING METHODS ####################
	static std::vector<Light> load(const std::string& filename);
};

}

#endif
