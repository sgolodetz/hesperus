/***
 * hesperus: NavFile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavFile.h"

#include <fstream>

#include <source/exceptions/Exception.h>
#include <source/io/sections/NavSection.h>

namespace hesp {

//#################### LOADING METHODS ####################
std::vector<NavDataset_Ptr> NavFile::load(const std::string& filename)
{
	std::ifstream is(filename.c_str(), std::ios_base::binary);
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");
	return NavSection::load(is);
}

//#################### SAVING METHODS ####################
void NavFile::save(const std::string& filename, const std::vector<NavDataset_Ptr>& datasets)
{
	std::ofstream os(filename.c_str(), std::ios_base::binary);
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");
	NavSection::save(os, datasets);
}

}
