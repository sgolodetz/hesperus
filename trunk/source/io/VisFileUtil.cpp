/***
 * hesperus: VisFileUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "VisFileUtil.h"

#include <fstream>

#include "FileSectionUtil.h"

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads a leaf visibility table from the specified file.

@param filename	The name of the file from which to load the visibility table
@return			The visibility table
*/
LeafVisTable_Ptr VisFileUtil::load(const std::string& filename)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("The vis file could not be read");
	return FileSectionUtil::load_vis_section(is);
}

//#################### SAVING METHODS ####################
/**
Saves a vis table file.

@param filename	The name of the file to which to save the visibility table
@param leafVis	The visibility table
*/
void VisFileUtil::save(const std::string& filename, const LeafVisTable_Ptr& leafVis)
{
	std::ofstream os(filename.c_str());
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");
	FileSectionUtil::save_vis_section(os, leafVis);
}

}
