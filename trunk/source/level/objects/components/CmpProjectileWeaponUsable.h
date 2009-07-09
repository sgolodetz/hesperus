/***
 * hesperus: CmpProjectileWeaponUsable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPPROJECTILEWEAPONUSABLE
#define H_HESP_CMPPROJECTILEWEAPONUSABLE

#include "CmpUsable.h"

namespace hesp {

class CmpProjectileWeaponUsable : public CmpUsable
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_projectileType;

	//#################### CONSTRUCTORS ####################
public:
	CmpProjectileWeaponUsable(const std::string& usableGroup, const std::vector<std::string>& hotspots, const std::string& projectileType);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	std::pair<std::string,Properties> save() const;
	void use();
};

}

#endif
