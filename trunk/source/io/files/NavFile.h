/***
 * hesperus: NavFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVFILE
#define H_HESP_NAVFILE

#include <string>

#include <source/level/nav/NavDataset.h>

namespace hesp {

struct NavFile
{
	//#################### LOADING METHODS ####################
	static std::vector<NavDataset_Ptr> load(const std::string& filename);

	//#################### SAVING METHODS ####################
	static void save(const std::string& filename, const std::vector<NavDataset_Ptr>& datasets);
};

}

#endif