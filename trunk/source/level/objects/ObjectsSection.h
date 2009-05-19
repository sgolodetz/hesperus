/***
 * hesperus: ObjectsSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTSSECTION
#define H_HESP_OBJECTSSECTION

#include <boost/filesystem/operations.hpp>

#include <ASXEngine.h>

#include "ObjectManager.h"

namespace hesp {

class ObjectsSection
{
	//#################### LOADING METHODS ####################
public:
	static ObjectManager_Ptr load(std::istream& is, const std::vector<AABB3d>& aabbs, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes, const boost::filesystem::path& baseDir);

	// TODO

	//#################### LOADING SUPPORT METHODS ####################
private:
	static std::vector<IComponent_Ptr> load_object(std::istream& is, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes, const ASXEngine_Ptr& aiEngine, const boost::filesystem::path& baseDir);
	static std::string lookup_property_type(const std::string& componentName, const std::string& propertyName, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes);
	static std::vector<int> string_to_intarray(const std::string& s);

	//#################### COMPONENT CREATOR TYPEDEFS ####################
private:
	typedef IComponent_Ptr (*ComponentCreator)(const Properties&);

	//#################### COMPONENT CREATOR METHODS ####################
private:
	static std::map<std::string,ComponentCreator>& component_creators();
	static IComponent_Ptr invoke_component_creator(const std::string& componentName, const Properties& properties);
};

}

#endif
