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
	HealthComponent(std::istream& is);

	//#################### PUBLIC METHODS ####################
public:
	int health() const;
	void save(std::ostream& os) const;
};

}

#endif
