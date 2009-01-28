/***
 * hesperus: HealthComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_HEALTHCOMPONENT
#define H_HESP_HEALTHCOMPONENT

#include <source/io/FieldIO.h>
#include "IHealthComponent.h"

namespace hesp {

class HealthComponent : public IHealthComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_health;

	//#################### CONSTRUCTORS ####################
public:
	HealthComponent(std::istream& is)
	{
		m_health = FieldIO::read_typed_field<int>(is, "Health");
	}

	//#################### PUBLIC METHODS ####################
public:
	int health() const
	{
		return m_health;
	}

	void save(std::ostream& os) const
	{
		FieldIO::write_typed_field(os, "Health", m_health);
	}
};

}

#endif
