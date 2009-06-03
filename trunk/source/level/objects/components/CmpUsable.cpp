/***
 * hesperus: CmpUsable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpUsable.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpUsable::CmpUsable(const std::string& usableGroup, const std::string& scriptName)
:	m_usableGroup(usableGroup), m_scriptName(scriptName)
{
	// TODO
}

//#################### STATIC FACTORY METHODS ####################
IComponent_Ptr CmpUsable::load(const Properties& properties)
{
	return IComponent_Ptr(new CmpUsable(properties.get_actual<std::string>("Group"), properties.get_actual<std::string>("Script")));
}

//#################### PUBLIC METHODS ####################
std::pair<std::string,Properties> CmpUsable::save() const
{
	Properties properties;
	properties.set_actual("Group", m_usableGroup);
	properties.set_actual("Script", m_scriptName);
	return std::make_pair("Usable", properties);
}

std::string CmpUsable::usable_group() const
{
	return m_usableGroup;
}

void CmpUsable::use()
{
	// TODO
}

}
