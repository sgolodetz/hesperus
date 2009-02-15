/***
 * hesperus: EntityComponents.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntityComponents.h"

#include <source/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
EntityComponents::EntityComponents()
{
	add_entry("VariableCamera");
	add_entry("Collision");
	add_entry("Health");
	add_entry("Nav");
	add_entry("Physics");
	add_entry("Visibility");
	add_entry("Yoke");
}

//#################### PUBLIC METHODS ####################
bool EntityComponents::has(const std::string& name) const
{
	std::map<std::string,bool>::const_iterator it = m_components.find(name);
	if(it != m_components.end()) return it->second;
	else throw Exception(name + " is not a valid entity component name");
}

void EntityComponents::set(const std::string& name, bool value)
{
	std::map<std::string,bool>::iterator it = m_components.find(name);
	if(it != m_components.end()) it->second = value;
	else throw Exception(name + " is not a valid entity component name");
}

//#################### PRIVATE METHODS ####################
void EntityComponents::add_entry(const std::string& name)
{
	m_components.insert(std::make_pair(name, false));
}

}
