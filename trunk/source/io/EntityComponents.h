/***
 * hesperus: EntityComponents.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITYCOMPONENTS
#define H_HESP_ENTITYCOMPONENTS

#include <map>
#include <string>

#include <source/exceptions/Exception.h>

namespace hesp {

/**
This class records the types of component contained in a given entity.
*/
class EntityComponents
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::map<std::string,bool> m_components;

	//#################### CONSTRUCTORS ####################
public:
	EntityComponents()
	{
		add_entry("Camera");
		add_entry("Collision");
		add_entry("Health");
		add_entry("Nav");
		add_entry("Physics");
		add_entry("Visibility");
		add_entry("Yoke");
	}

	//#################### PUBLIC METHODS ####################
public:
	bool has(const std::string& name) const
	{
		std::map<std::string,bool>::const_iterator it = m_components.find(name);
		if(it != m_components.end()) return it->second;
		else throw Exception(name + " is not a valid entity component name");
	}

	void set(const std::string& name, bool value)
	{
		std::map<std::string,bool>::iterator it = m_components.find(name);
		if(it != m_components.end()) it->second = value;
		else throw Exception(name + " is not a valid entity component name");
	}

	//#################### PRIVATE METHODS ####################
private:
	void add_entry(const std::string& name)
	{
		m_components.insert(std::make_pair(name, false));
	}
};

}

#endif
