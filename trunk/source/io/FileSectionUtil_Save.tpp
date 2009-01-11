/***
 * hesperus: FileSectionUtil_Save.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### SAVING METHODS ####################
/**
TODO
*/
template <typename Poly>
void FileSectionUtil::save_polygons_section(std::ostream& os, const std::string& sectionName, const std::vector<shared_ptr<Poly> >& polygons)
{
	os << sectionName << '\n';
	os << "{\n";
	write_polygons(os, polygons, true);
	os << "}\n";
}

}
