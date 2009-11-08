/***
 * hesperus: CmpProjectileWeaponUsable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpProjectileWeaponUsable.h"

#include <source/level/objects/messages/MsgTimeElapsed.h>
#include <source/util/Properties.h>
#include "ICmpOwnable.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpProjectileWeaponUsable::CmpProjectileWeaponUsable(int firingInterval, const std::string& usableGroup, const std::vector<std::string>& hotspots, double muzzleSpeed,
													 const std::string& projectileType, int timeTillCanFire)
:	CmpUsable(usableGroup, hotspots), m_firingInterval(firingInterval), m_muzzleSpeed(muzzleSpeed), m_projectileType(projectileType), m_timeTillCanFire(timeTillCanFire)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpProjectileWeaponUsable::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpProjectileWeaponUsable(properties.get<int>("FiringInterval"),
															  properties.get<std::string>("Group"),
															  properties.get<std::vector<std::string> >("Hotspots"),
															  properties.get<double>("MuzzleSpeed"),
															  properties.get<std::string>("ProjectileType"),
															  properties.get<int>("TimeTillCanFire")));
}

//#################### PUBLIC METHODS ####################
void CmpProjectileWeaponUsable::process_message(const MsgTimeElapsed& msg)
{
	m_timeTillCanFire -= msg.milliseconds();
	if(m_timeTillCanFire < 0) m_timeTillCanFire = 0;
}

Properties CmpProjectileWeaponUsable::save() const
{
	Properties properties;
	properties.set("FiringInterval", m_firingInterval);
	properties.set("Group", usable_group());
	properties.set("Hotspots", hotspots());
	properties.set("MuzzleSpeed", m_muzzleSpeed);
	properties.set("ProjectileType", m_projectileType);
	properties.set("TimeTillCanFire", m_timeTillCanFire);
	return properties;
}

void CmpProjectileWeaponUsable::use()
{
	if(m_timeTillCanFire == 0)
	{
		// TODO: Check that there's enough ammo.

		// Determine the character which is firing the projectile (the owner of the weapon).
		ICmpOwnable_CPtr cmpOwnable = m_objectManager->get_component(m_objectID, cmpOwnable);
		ObjectID firer = cmpOwnable != NULL ? cmpOwnable->owner() : ObjectID();

		// Fire a bullet from each hotspot of the weapon (note that this in principle makes it easy to implement things like double-barrelled shotguns).
		const std::vector<std::string>& spots = hotspots();
		for(size_t i=0, size=spots.size(); i<size; ++i)
		{
			boost::optional<Vector3d> pos = hotspot_position(spots[i]);
			boost::optional<Vector3d> ori = hotspot_orientation(spots[i]);
			if(pos && ori)
			{
				ObjectSpecification specification = m_objectManager->get_archetype(m_projectileType);
				specification.set_component_property("Projectile", "Firer", firer);
				specification.set_component_property("Simulation", "Position", *pos);
				specification.set_component_property("Simulation", "Velocity", m_muzzleSpeed * *ori);
				m_objectManager->queue_for_construction(specification);
			}
		}

		m_timeTillCanFire = m_firingInterval;
	}
}

}
