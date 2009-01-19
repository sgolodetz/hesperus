/***
 * hesperus: NavFileUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavFileUtil.h"

#include <fstream>

#include <source/exceptions/Exception.h>
#include "FileSectionUtil.h"

namespace hesp {

//#################### SAVING METHODS ####################
void NavFileUtil::save(const std::string& filename, const std::vector<NavDataset_Ptr>& datasets)
{
	std::ofstream os(filename.c_str(), std::ios_base::binary);
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");

	FileSectionUtil::save_nav_section(os, datasets);
}

}
