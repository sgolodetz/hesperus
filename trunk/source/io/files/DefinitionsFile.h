/***
 * hesperus: DefinitionsFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_DEFINITIONSFILE
#define H_HESP_DEFINITIONSFILE

#include <string>
#include <vector>

#include <source/math/geom/AABB.h>
#include <source/xml/XMLParser.h>

namespace hesp {

class DefinitionsFile
{
	//#################### LOADING METHODS ####################
public:
	static std::vector<AABB3d> load_aabbs_only(const std::string& filename);

	//#################### LOADING SUPPORT METHODS ####################
private:
	static Vector3d extract_vector3d(const XMLElement_CPtr& elt);
};

}

#endif