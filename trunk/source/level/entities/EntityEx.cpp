/***
 * hesperus: EntityEx.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntityEx.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
EntityEx::EntityEx(const std::string& archetype)
:	m_id(-1), m_archetype(archetype)
{}

//#################### PUBLIC METHODS ####################
int EntityEx::id() const			{ return m_id; }

void EntityEx::save(std::ostream& os) const
{
	os << "Instance " << m_archetype << '\n';
	os << "{\n";

	m_collisionComponent->save(os);
	m_healthComponent->save(os);
	m_lookComponent->save(os);
	m_positionComponent->save(os);
	m_visibilityComponent->save(os);

	os << "}\n";
}

void EntityEx::set_id(int id)		{ m_id = id; }

}
