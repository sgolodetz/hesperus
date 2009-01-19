/***
 * hesperus: NavFileUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IO_NAVFILEUTIL
#define H_HESP_IO_NAVFILEUTIL

#include <string>

#include <source/level/nav/NavDataset.h>

namespace hesp {

struct NavFileUtil
{
	//#################### LOADING METHODS ####################
	// TODO

	//#################### SAVING METHODS ####################
	static void save(const std::string& filename, const std::vector<NavDataset_Ptr>& datasets);
};

}

#endif
