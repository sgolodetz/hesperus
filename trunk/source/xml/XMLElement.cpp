/***
 * hesperus: XMLElement.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "XMLElement.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
XMLElement::XMLElement(const std::string& name)
:	m_name(name)
{}

//#################### PUBLIC METHODS ####################
void XMLElement::add_child(const XMLElement_Ptr& child)
{
	m_children.push_back(child);
}

const std::string& XMLElement::name() const
{
	return m_name;
}

void XMLElement::set_attribute(const std::string& name, const std::string& value)
{
	m_attributes[name] = value;
}

}
