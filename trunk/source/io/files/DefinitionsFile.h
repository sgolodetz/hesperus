/***
 * hesperus: DefinitionsFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_DEFINITIONSFILE
#define H_HESP_DEFINITIONSFILE

#include <map>
#include <string>
#include <vector>

#include <source/math/geom/AABB.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class XMLElement> XMLElement_CPtr;

class DefinitionsFile
{
	//#################### LOADING METHODS ####################
public:
	static void load(const std::string& filename, std::vector<AABB3d>& aabbs, std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes);
	static std::vector<AABB3d> load_aabbs_only(const std::string& filename);

	//#################### LOADING SUPPORT METHODS ####################
private:
	static Vector3d extract_vector3d(const XMLElement_CPtr& elt);
};

}

#endif
