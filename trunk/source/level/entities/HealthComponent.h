/***
 * hesperus: HealthComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_HEALTHCOMPONENT
#define H_HESP_HEALTHCOMPONENT

#include "IHealthComponent.h"

namespace hesp {

class HealthComponent : public IHealthComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_health;

	//#################### CONSTRUCTORS ####################
public:
	HealthComponent(int health)
	:	m_health(health)
	{}

	//#################### PUBLIC METHODS ####################
public:
	int health() const
	{
		return m_health;
	}
};

}

#endif
