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
	static std::vector<IComponent_Ptr> load_object(std::istream& is, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes, const ASXEngine_Ptr& aiEngine, const boost::filesystem::path& baseDir);
	static std::string lookup_property_type(const std::string& componentName, const std::string& propertyName, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes);
	static std::vector<int> string_to_intarray(const std::string& s);

	//#################### SAVING SUPPORT METHODS ####################
private:
	static std::string intarray_to_string(const std::vector<int>& arr);
	static void save_object(std::ostream& os, const std::vector<IComponent_Ptr>& components, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes);

	//#################### COMPONENT LOADER TYPEDEFS ####################
private:
	typedef IComponent_Ptr (*ComponentLoader)(const Properties&);

	//#################### COMPONENT LOADER METHODS ####################
private:
	static std::map<std::string,ComponentLoader>& component_loaders();
	static IComponent_Ptr invoke_component_loader(const std::string& componentName, const Properties& properties);
};

}

#endif
