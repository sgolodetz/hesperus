/***
 * hesperus: ObjectSpecification.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTSPECIFICATION
#define H_HESP_OBJECTSPECIFICATION

#include <map>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class IObjectComponent> IObjectComponent_Ptr;
class Properties;

class ObjectSpecification
{
	//#################### TYPEDEFS ####################
private:
	typedef std::map<std::string,Properties> ComponentMap;

	//#################### PRIVATE VARIABLES ####################
private:
	ComponentMap m_components;

	//#################### PUBLIC METHODS ####################
public:
	void add_component(const std::string& componentName, const Properties& properties);
	std::vector<IObjectComponent_Ptr> instantiate_components() const;

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
