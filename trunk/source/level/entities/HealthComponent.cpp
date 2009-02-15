/***
 * hesperus: HealthComponent.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "HealthComponent.h"

#include <source/io/FieldIO.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
HealthComponent::HealthComponent(std::istream& is)
{
	m_health = FieldIO::read_typed_field<int>(is, "Health");
}

//#################### PUBLIC METHODS ####################
int HealthComponent::health() const
{
	return m_health;
}

void HealthComponent::save(std::ostream& os) const
{
	FieldIO::write_typed_field(os, "Health", m_health);
}

}
