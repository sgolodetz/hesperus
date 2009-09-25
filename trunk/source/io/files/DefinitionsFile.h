/***
 * hesperus: DefinitionsFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_DEFINITIONSFILE
#define H_HESP_DEFINITIONSFILE

#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Bounds> Bounds_Ptr;
typedef shared_ptr<class BoundsManager> BoundsManager_Ptr;
typedef shared_ptr<const class XMLElement> XMLElement_CPtr;

class DefinitionsFile
{
	//#################### LOADING METHODS ####################
public:
	static void load(const std::string& filename, BoundsManager_Ptr& boundsManager, std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes);
	static BoundsManager_Ptr load_bounds_only(const std::string& filename);

	//#################### LOADING SUPPORT METHODS ####################
private:
	static Bounds_Ptr load_aabb_bounds(const XMLElement_CPtr& elt);
	static BoundsManager_Ptr load_bounds(const XMLElement_CPtr& boundsElt);
	static Bounds_Ptr load_point_bounds(const XMLElement_CPtr&);
	static Bounds_Ptr load_sphere_bounds(const XMLElement_CPtr& elt);
};

}

#endif
