/***
 * hesperus: NavFileUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavFileUtil.h"

#include <fstream>

#include <source/exceptions/Exception.h>
#include "FileSectionUtil.h"

namespace hesp {

//#################### LOADING METHODS ####################
std::vector<NavDataset_Ptr> NavFileUtil::load(const std::string& filename)
{
	std::ifstream is(filename.c_str(), std::ios_base::binary);
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	return FileSectionUtil::load_nav_section(is);
}

//#################### SAVING METHODS ####################
void NavFileUtil::save(const std::string& filename, const std::vector<NavDataset_Ptr>& datasets)
{
	std::ofstream os(filename.c_str(), std::ios_base::binary);
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");

	FileSectionUtil::save_nav_section(os, datasets);
}

}
