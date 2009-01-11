/***
 * hesperus: PortalsFileUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IO_PORTALSFILEUTIL
#define H_HESP_IO_PORTALSFILEUTIL

#include <string>
#include <vector>

#include <source/level/portals/Portal.h>

namespace hesp {

struct PortalsFileUtil
{
	//#################### LOADING METHODS ####################
	static void load(const std::string& filename, int& emptyLeafCount, std::vector<Portal_Ptr>& portals);

	//#################### SAVING METHODS ####################
	static void save(const std::string& filename, int emptyLeafCount, const std::vector<Portal_Ptr>& portals);
};

}

#endif
