/***
 * hesperus: CmpScriptedUsable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpScriptedUsable.h"

#if 0
// TEMPORARY
#include <iostream>
#endif

#include "ICmpModelRender.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpScriptedUsable::CmpScriptedUsable(const std::string& usableGroup, const std::vector<std::string>& hotspots, const std::string& scriptName)
:	m_usableGroup(usableGroup), m_hotspots(hotspots), m_scriptName(scriptName)
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
void CmpScriptedUsable::check_dependencies() const
{
	check_dependency<ICmpModelRender>();
}

const std::vector<std::string>& CmpScriptedUsable::hotspots() const
{
	return m_hotspots;
}

boost::optional<Vector3d> CmpScriptedUsable::hotspot_orientation(const std::string& name) const
{
	verify_hotspot_name(name);
	std::map<std::string,Vector3d>::const_iterator it = m_hotspotOrientations.find(name);
	if(it != m_hotspotOrientations.end()) return it->second;
	else return boost::none;
}

boost::optional<Vector3d> CmpScriptedUsable::hotspot_position(const std::string& name) const
{
	verify_hotspot_name(name);
	std::map<std::string,Vector3d>::const_iterator it = m_hotspotPositions.find(name);
	if(it != m_hotspotPositions.end()) return it->second;
	else return boost::none;
}

void CmpScriptedUsable::set_hotspot_orientation(const std::string& name, const Vector3d& orientation)
{
	verify_hotspot_name(name);
	m_hotspotOrientations[name] = orientation;
}

void CmpScriptedUsable::set_hotspot_position(const std::string& name, const Vector3d& position)
{
	verify_hotspot_name(name);
	m_hotspotPositions[name] = position;
}

std::pair<std::string,Properties> CmpScriptedUsable::save() const
{
	Properties properties;
	properties.set("Group", m_usableGroup);
	properties.set("Hotspots", m_hotspots);
	properties.set("Script", m_scriptName);
	return std::make_pair("ScriptedUsable", properties);
}

std::string CmpScriptedUsable::usable_group() const
{
	return m_usableGroup;
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

//#################### PRIVATE METHODS ####################
void CmpScriptedUsable::verify_hotspot_name(const std::string& name) const
{
	if(std::find(m_hotspots.begin(), m_hotspots.end(), name) == m_hotspots.end())
	{
		throw Exception("No such hotspot: " + name);
	}
}

}
