/***
 * hesperus: OnionPortalsFileUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "OnionPortalsFileUtil.h"

#include <fstream>

#include "FileSectionUtil.h"

namespace hesp {

//#################### SAVING METHODS ####################
/**
Saves an array of onion portals to the specified file.

@param filename			The name of the onion portals file
@param portals			The onion portals
*/
void OnionPortalsFileUtil::save(const std::string& filename, const std::vector<OnionPortal_Ptr>& portals)
{
	std::ofstream os(filename.c_str());
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");
	FileSectionUtil::save_polygons_section(os, "OnionPortals", portals);
}

}
