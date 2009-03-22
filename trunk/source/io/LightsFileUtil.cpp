/***
 * hesperus: LightsFileUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "LightsFileUtil.h"

#include <fstream>

#include <source/exceptions/Exception.h>
#include <source/io/sections/LightsSection.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads an array of lights from the specified file.

@param filename	The name of the file containing the list of lights
@return			The array of lights
*/
std::vector<Light> LightsFileUtil::load(const std::string& filename)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");
	return LightsSection::load(is);
}

//#################### SAVING METHODS ####################
void LightsFileUtil::save(const std::string& filename, const std::vector<Light>& lights)
{
	std::ofstream os(filename.c_str());
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");
	LightsSection::save(os, lights);
}

}
