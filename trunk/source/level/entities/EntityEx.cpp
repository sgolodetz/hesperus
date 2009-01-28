/***
 * hesperus: EntityEx.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntityEx.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
EntityEx::EntityEx(const std::string& entityClass)
:	m_id(-1), m_entityClass(entityClass)
{}

//#################### PUBLIC METHODS ####################
ICollisionComponent_Ptr EntityEx::collision_component()		{ return m_collisionComponent; }
IHealthComponent_Ptr EntityEx::health_component()			{ return m_healthComponent; }
int EntityEx::id() const									{ return m_id; }
ILookComponent_Ptr EntityEx::look_component()				{ return m_lookComponent; }
IPositionComponent_Ptr EntityEx::position_component()		{ return m_positionComponent; }
IVisibilityComponent_Ptr EntityEx::visibility_component()	{ return m_visibilityComponent; }
IYokeComponent_Ptr EntityEx::yoke_component()				{ return m_yokeComponent; }

void EntityEx::save(std::ostream& os) const
{
	os << "Instance " << m_entityClass << '\n';
	os << "{\n";

	m_collisionComponent->save(os);
	m_healthComponent->save(os);
	m_lookComponent->save(os);
	m_positionComponent->save(os);
	m_visibilityComponent->save(os);

	os << "}\n";
}

void EntityEx::set_collision_component(const ICollisionComponent_Ptr& collisionComponent)		{ m_collisionComponent = collisionComponent; }
void EntityEx::set_health_component(const IHealthComponent_Ptr& healthComponent)				{ m_healthComponent = healthComponent; }
void EntityEx::set_id(int id)																	{ m_id = id; }
void EntityEx::set_look_component(const ILookComponent_Ptr& lookComponent)						{ m_lookComponent = lookComponent; }
void EntityEx::set_position_component(const IPositionComponent_Ptr& positionComponent)			{ m_positionComponent = positionComponent; }
void EntityEx::set_visibility_component(const IVisibilityComponent_Ptr& visibilityComponent)	{ m_visibilityComponent = visibilityComponent; }
void EntityEx::set_yoke_component(const IYokeComponent_Ptr& yokeComponent)						{ m_yokeComponent = yokeComponent; }

}
