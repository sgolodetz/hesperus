/***
 * hesperus: ObjectsSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTSSECTION
#define H_HESP_OBJECTSSECTION

#include <map>

#include <boost/filesystem/operations.hpp>

#include <source/io/util/FieldIO.h>
#include <source/level/objects/base/ObjectSpecification.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class BoundsManager> BoundsManager_CPtr;
class ComponentPropertyTypeMap;
typedef shared_ptr<class IObjectComponent> IObjectComponent_Ptr;
typedef shared_ptr<class ObjectManager> ObjectManager_Ptr;
class Properties;

class ObjectsSection
{
	//#################### LOADING METHODS ####################
public:
	static ObjectManager_Ptr load(std::istream& is, const BoundsManager_CPtr& boundsManager, const ComponentPropertyTypeMap& componentPropertyTypes, const std::map<std::string,ObjectSpecification>& archetypes, const boost::filesystem::path& baseDir);

	//#################### SAVING METHODS ####################
public:
	static void save(std::ostream& os, const ObjectManager_Ptr& objectManager);

	//#################### LOADING SUPPORT METHODS ####################
private:
	static ObjectSpecification load_object_specification(std::istream& is, const ComponentPropertyTypeMap& componentPropertyTypes, const boost::filesystem::path& baseDir);

	//#################### SAVING SUPPORT METHODS ####################
private:
	static void save_object(std::ostream& os, const std::vector<IObjectComponent_Ptr>& components, const ComponentPropertyTypeMap& componentPropertyTypes);
};

}

#endif
