/***
 * hesperus: CmpItemActivatable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpItemActivatable.h"

#include <iostream>

namespace hesp {

//#################### STATIC FACTORY METHODS ####################
IComponent_Ptr CmpItemActivatable::load(const Properties&)
{
	return IComponent_Ptr(new CmpItemActivatable);
}

//#################### PUBLIC METHODS ####################
void CmpItemActivatable::activated_by(const ObjectID& activator)
{
	std::cout << "Object " << m_objectID.to_string() << " activated by object " << activator.to_string() << std::endl;
}

std::pair<std::string,Properties> CmpItemActivatable::save() const
{
	return std::make_pair("ItemActivatable", Properties());
}

}
