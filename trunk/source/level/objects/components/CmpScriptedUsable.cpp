/***
 * hesperus: CmpScriptedUsable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpScriptedUsable.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpScriptedUsable::CmpScriptedUsable(const std::string& usableGroup, const std::vector<std::string>& hotspots, const std::string& scriptName)
:	CmpUsable(usableGroup, hotspots), m_scriptName(scriptName)
{
	// TODO: Script-related stuff
}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpScriptedUsable::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpScriptedUsable(properties.get<std::string>("Group"),
													  properties.get<std::vector<std::string> >("Hotspots"),
													  properties.get<std::string>("Script")));
}

//#################### PUBLIC METHODS ####################
std::pair<std::string,Properties> CmpScriptedUsable::save() const
{
	Properties properties;
	properties.set("Group", usable_group());
	properties.set("Hotspots", hotspots());
	properties.set("Script", m_scriptName);
	return std::make_pair("ScriptedUsable", properties);
}

void CmpScriptedUsable::use()
{
	// TODO
}

}
