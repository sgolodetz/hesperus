/***
 * hesperus: CmpScriptedUsable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpScriptedUsable.h"

#if 0
// TEMPORARY
#include <iostream>
#endif

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
	properties.set("Group", m_usableGroup);
	properties.set("Hotspots", m_hotspots);
	properties.set("Script", m_scriptName);
	return std::make_pair("ScriptedUsable", properties);
}

void CmpScriptedUsable::use()
{
#if 0
	// TEMPORARY
	boost::optional<Vector3d> pos = hotspot_position("root");
	boost::optional<Vector3d> ori = hotspot_orientation("root");
	if(pos && ori)
	{
		std::cout << "Creating bullet at position " << *pos << " with orientation " << *ori << std::endl;
	}
	else
	{
		std::cout << "Could not create bullet: hotspot 'root' not initialised yet" << std::endl;
	}
#endif

	// TODO
}

}
