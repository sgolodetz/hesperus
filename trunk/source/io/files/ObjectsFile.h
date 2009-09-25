/***
 * hesperus: ObjectsFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTSFILE
#define H_HESP_OBJECTSFILE

#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/operations.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class BoundsManager> BoundsManager_CPtr;
typedef shared_ptr<class ObjectManager> ObjectManager_Ptr;

struct ObjectsFile
{
	//#################### LOADING METHODS ####################
	static ObjectManager_Ptr load(const std::string& filename, const BoundsManager_CPtr& boundsManager, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes, const boost::filesystem::path& baseDir);

	//#################### SAVING METHODS ####################
	static void save(const std::string& filename, const ObjectManager_Ptr& objectManager);
};

}

#endif
