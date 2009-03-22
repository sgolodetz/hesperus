/***
 * hesperus: EntityComponents.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITYCOMPONENTS
#define H_HESP_ENTITYCOMPONENTS

#include <map>
#include <string>

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
	EntityComponents();

	//#################### PUBLIC METHODS ####################
public:
	bool has(const std::string& name) const;
	void set(const std::string& name, bool value);

	//#################### PRIVATE METHODS ####################
private:
	void add_entry(const std::string& name);
};

}

#endif
