/***
 * hesperus: EntDefFileUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntDefFileUtil.h"

#include <fstream>

#include <source/exceptions/Exception.h>
#include "FileSectionUtil.h"

namespace hesp {

//#################### LOADING METHODS ####################
std::vector<AABB3d> EntDefFileUtil::load_aabbs_only(const std::string& filename)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	FileSectionUtil::read_checked_line(is, "HENTDEF");
	return FileSectionUtil::load_aabbs_section(is);
}

}
