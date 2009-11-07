/***
 * hesperus: CmpProjectileWeaponUsable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpProjectileWeaponUsable.h"

#include <SDL.h>

#include <source/util/Properties.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpProjectileWeaponUsable::CmpProjectileWeaponUsable(int firingInterval, const std::string& usableGroup, const std::vector<std::string>& hotspots, double muzzleSpeed,
													 const std::string& projectileType)
:	CmpUsable(usableGroup, hotspots), m_firingInterval(firingInterval), m_muzzleSpeed(muzzleSpeed), m_projectileType(projectileType)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpProjectileWeaponUsable::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpProjectileWeaponUsable(properties.get<int>("FiringInterval"),
															  properties.get<std::string>("Group"),
															  properties.get<std::vector<std::string> >("Hotspots"),
															  properties.get<double>("MuzzleSpeed"),
															  properties.get<std::string>("ProjectileType")));
}

//#################### PUBLIC METHODS ####################
Properties CmpProjectileWeaponUsable::save() const
{
	Properties properties;
	properties.set("FiringInterval", m_firingInterval);
	properties.set("Group", usable_group());
	properties.set("Hotspots", hotspots());
	properties.set("MuzzleSpeed", m_muzzleSpeed);
	properties.set("ProjectileType", m_projectileType);
	return properties;
}

void CmpProjectileWeaponUsable::use()
{
	unsigned long curTime = SDL_GetTicks();

	// If the weapon's never been fired, or it's been at least the specified firing interval since it was fired,
	// or the time's wrapped around because we've been playing too long (> 49 days!), then fire the weapon.
	// Note that strictly speaking we should check that the appropriate amount of time has elapsed even if we
	// wrapped round, but it only happens after 49 days of continuous playing, so I'm not going to devote lots
	// of attention to it - if someone fires one bullet earlier than they should strictly be able to, it won't
	// ruin the game.
	if(!m_lastFired || curTime >= *m_lastFired + static_cast<unsigned long>(m_firingInterval) || curTime < *m_lastFired)
	{
		// Fire a bullet from each hotspot of the weapon (note that this in principle makes it easy to implement things like double-barrelled shotguns).
		const std::vector<std::string>& spots = hotspots();
		for(size_t i=0, size=spots.size(); i<size; ++i)
		{
			boost::optional<Vector3d> pos = hotspot_position(spots[i]);
			boost::optional<Vector3d> ori = hotspot_orientation(spots[i]);
			if(pos && ori)
			{
				ObjectSpecification specification = m_objectManager->get_archetype(m_projectileType);
				specification.set_component_property("Simulation", "Position", *pos);
				specification.set_component_property("Simulation", "Velocity", m_muzzleSpeed * *ori);
				m_objectManager->queue_for_construction(specification);
			}
		}

		m_lastFired = curTime;
	}
}

}
