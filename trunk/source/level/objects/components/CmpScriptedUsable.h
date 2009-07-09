/***
 * hesperus: CmpScriptedUsable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPSCRIPTEDUSABLE
#define H_HESP_CMPSCRIPTEDUSABLE

#include "ICmpUsable.h"

namespace hesp {

class CmpScriptedUsable : public ICmpUsable
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_usableGroup;
	std::vector<std::string> m_hotspots;
	std::map<std::string,Vector3d> m_hotspotOrientations;
	std::map<std::string,Vector3d> m_hotspotPositions;
	std::string m_scriptName;

	//#################### CONSTRUCTORS ####################
public:
	CmpScriptedUsable(const std::string& usableGroup, const std::vector<std::string>& hotspots, const std::string& scriptName);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	void check_dependencies() const;
	const std::vector<std::string>& hotspots() const;
	boost::optional<Vector3d> hotspot_orientation(const std::string& name) const;
	boost::optional<Vector3d> hotspot_position(const std::string& name) const;
	void set_hotspot_orientation(const std::string& name, const Vector3d& orientation);
	void set_hotspot_position(const std::string& name, const Vector3d& position);
	std::pair<std::string,Properties> save() const;
	std::string usable_group() const;
	void use();

	//#################### PRIVATE METHODS ####################
private:
	void verify_hotspot_name(const std::string& name) const;
};

}

#endif
