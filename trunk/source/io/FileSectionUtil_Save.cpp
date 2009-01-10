/***
 * hesperus: FileSectionUtil_Save.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FileSectionUtil.h"

namespace hesp {

//#################### SAVING METHODS ####################
/**
Saves a lightmap prefix to the specified std::ostream.

@param is	The std::ostream
@param		The lightmap prefix
*/
void FileSectionUtil::save_lightmap_prefix_section(std::ostream& os, const std::string& lightmapPrefix)
{
	os << "LightmapPrefix\n";
	os << "{\n";
	os << lightmapPrefix << '\n';
	os << "}\n";
}

}
