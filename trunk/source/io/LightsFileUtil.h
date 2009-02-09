/***
 * hesperus: LightsFileUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LIGHTSFILEUTIL
#define H_HESP_LIGHTSFILEUTIL

#include <vector>

#include <source/level/lighting/Light.h>

namespace hesp {

struct LightsFileUtil
{
	//#################### LOADING METHODS ####################
	static std::vector<Light> load(const std::string& filename);

	//#################### SAVING METHODS ####################
	static void save(const std::string& filename, const std::vector<Light>& lights);
};

}

#endif
