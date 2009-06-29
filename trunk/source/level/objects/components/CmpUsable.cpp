/***
 * hesperus: CmpUsable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpUsable.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpUsable::CmpUsable(const std::string& usableGroup, const std::vector<std::string>& hotspots, const std::string& scriptName)
:	m_usableGroup(usableGroup), m_hotspots(hotspots), m_scriptName(scriptName)
{
	// TODO
}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpUsable::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpUsable(properties.get<std::string>("Group"),
											  properties.get<std::vector<std::string> >("Hotspots"),
											  properties.get<std::string>("Script")));
}

//#################### PUBLIC METHODS ####################
const std::vector<std::string>& CmpUsable::hotspots() const
{
	return m_hotspots;
}

std::pair<std::string,Properties> CmpUsable::save() const
{
	Properties properties;
	properties.set("Group", m_usableGroup);
	properties.set("Hotspots", m_hotspots);
	properties.set("Script", m_scriptName);
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
