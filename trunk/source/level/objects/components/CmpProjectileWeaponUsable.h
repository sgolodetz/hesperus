/***
 * hesperus: CmpProjectileWeaponUsable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPPROJECTILEWEAPONUSABLE
#define H_HESP_CMPPROJECTILEWEAPONUSABLE

#include <boost/optional.hpp>

#include "CmpUsable.h"

namespace hesp {

class CmpProjectileWeaponUsable : public CmpUsable
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_firingInterval;							// the interval between firing one shot and the next (in milliseconds)
	boost::optional<unsigned long> m_lastFired;		// the time at which the weapon was last fired
	double m_muzzleSpeed;
	std::string m_projectileType;

	//#################### CONSTRUCTORS ####################
public:
	CmpProjectileWeaponUsable(int firingInterval, const std::string& usableGroup, const std::vector<std::string>& hotspots, double muzzleSpeed, const std::string& projectileType);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	Properties save() const;
	void use();

	std::string own_type() const			{ return "ProjectileWeaponUsable"; }
	static std::string static_own_type()	{ return "ProjectileWeaponUsable"; }
};

}

#endif
