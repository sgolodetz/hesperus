/***
 * hesperus: CmpProjectileWeaponUsable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpProjectileWeaponUsable.h"

#if 0
// TEMPORARY
#include <iostream>
#endif

#include <source/util/Properties.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpProjectileWeaponUsable::CmpProjectileWeaponUsable(const std::string& usableGroup, const std::vector<std::string>& hotspots, const std::string& projectileType)
:	CmpUsable(usableGroup, hotspots), m_projectileType(projectileType)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpProjectileWeaponUsable::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpProjectileWeaponUsable(properties.get<std::string>("Group"),
															  properties.get<std::vector<std::string> >("Hotspots"),
															  properties.get<std::string>("ProjectileType")));
}

//#################### PUBLIC METHODS ####################
Properties CmpProjectileWeaponUsable::save() const
{
	Properties properties;
	properties.set("Group", usable_group());
	properties.set("Hotspots", hotspots());
	properties.set("ProjectileType", m_projectileType);
	return properties;
}

void CmpProjectileWeaponUsable::use()
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
