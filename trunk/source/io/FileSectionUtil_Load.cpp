/***
 * hesperus: FileSectionUtil_Load.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FileSectionUtil.h"

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads a lightmap prefix from the specified std::istream.

@param is			The std::istream
@return				The lightmap prefix
@throws Exception	If EOF is encountered whilst trying to read the lightmap prefix
*/
std::string FileSectionUtil::load_lightmap_prefix_section(std::istream& is)
{
	std::string line, lightmapPrefix;

	read_line(is, line, "Unexpected EOF whilst trying to read LightmapPrefix");
	read_line(is, line, "Unexpected EOF whilst trying to read {");
	read_line(is, lightmapPrefix, "Unexpected EOF whilst trying to read lightmap prefix");
	read_line(is, line, "Unexpected EOF whilst trying to read }");

	return lightmapPrefix;
}

//#################### LOADING SUPPORT METHODS ####################
/**
Attempts to read a line from a std::istream into a string.

@param is			The std::istream
@param line			The string into which to read
@param errMsg		The error message to associate with failure
@throws Exception	If the read fails
*/
void FileSectionUtil::read_line(std::istream& is, std::string& line, const std::string& errMsg)
{
	if(!std::getline(is, line)) throw Exception(errMsg);
}

}
