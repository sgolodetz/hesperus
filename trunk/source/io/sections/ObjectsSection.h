/***
 * hesperus: ObjectsSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTSSECTION
#define H_HESP_OBJECTSSECTION

#include <boost/filesystem/operations.hpp>

#include <ASXEngine.h>

#include <source/level/objects/base/ObjectManager.h>

namespace hesp {

class ObjectsSection
{
	//#################### LOADING METHODS ####################
public:
	static ObjectManager_Ptr load(std::istream& is, const std::vector<AABB3d>& aabbs, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes, const boost::filesystem::path& baseDir);

	//#################### SAVING METHODS ####################
public:
	static void save(std::ostream& os, const ObjectManager_Ptr& objectManager);

	//#################### LOADING SUPPORT METHODS ####################
private:
	static std::vector<IObjectComponent_Ptr> load_object(std::istream& is, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes, const ASXEngine_Ptr& aiEngine, const boost::filesystem::path& baseDir);
	static std::string lookup_property_type(const std::string& componentName, const std::string& propertyName, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes);

	//#################### SAVING SUPPORT METHODS ####################
private:
	static void save_object(std::ostream& os, const std::vector<IObjectComponent_Ptr>& components, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes);

	//#################### COMPONENT LOADER TYPEDEFS ####################
private:
	typedef IObjectComponent_Ptr (*ComponentLoader)(const Properties&);

	//#################### COMPONENT LOADER METHODS ####################
private:
	static std::map<std::string,ComponentLoader>& component_loaders();
	static IObjectComponent_Ptr invoke_component_loader(const std::string& componentName, const Properties& properties);
};

}

#endif
