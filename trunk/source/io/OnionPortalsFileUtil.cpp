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
Loads an array of onion portals from the specified file.

@param filename		The name of the onion portals file
@return				The onion portals
*/
std::vector<OnionPortal_Ptr> OnionPortalsFileUtil::load(const std::string& filename)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	std::vector<OnionPortal_Ptr> portals;
	FileSectionUtil::load_polygons_section(is, "OnionPortals", portals);
	return portals;
}

//#################### SAVING METHODS ####################
/**
Saves an array of onion portals to the specified file.

@param filename		The name of the onion portals file
@param portals		The onion portals
*/
void OnionPortalsFileUtil::save(const std::string& filename, const std::vector<OnionPortal_Ptr>& portals)
{
	std::ofstream os(filename.c_str());
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");
	FileSectionUtil::save_polygons_section(os, "OnionPortals", portals);
}

}
