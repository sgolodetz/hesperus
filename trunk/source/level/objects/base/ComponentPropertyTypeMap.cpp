/***
 * hesperus: ComponentPropertyTypeMap.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ComponentPropertyTypeMap.h"

#include <source/exceptions/Exception.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
void ComponentPropertyTypeMap::insert(const std::string& componentName, const std::string& propertyName, const std::string& type)
{
	std::map<std::string,std::string>& propertyTypes = m_componentPropertyTypes[componentName];
	propertyTypes.insert(std::make_pair(propertyName, type));
}

const std::string& ComponentPropertyTypeMap::lookup(const std::string& componentName, const std::string& propertyName) const
{
	std::map<std::string,std::map<std::string,std::string> >::const_iterator it = m_componentPropertyTypes.find(componentName);
	if(it == m_componentPropertyTypes.end()) throw Exception("Unknown component: " + componentName);

	std::map<std::string,std::string>::const_iterator jt = it->second.find(propertyName);
	if(jt == it->second.end()) throw Exception("Unknown property (in component " + componentName + "): " + propertyName);

	return jt->second;
}

const std::map<std::string,std::string>& ComponentPropertyTypeMap::property_types(const std::string& componentName) const
{
	std::map<std::string,std::map<std::string,std::string> >::const_iterator it = m_componentPropertyTypes.find(componentName);
	if(it != m_componentPropertyTypes.end()) return it->second;
	else throw Exception("Unknown component: " + componentName);
}

}
