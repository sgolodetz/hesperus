/***
 * hesperus: Entity.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntityEx.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Entity::Entity(const std::string& entityClass)
:	m_id(-1), m_entityClass(entityClass)
{}

//#################### PUBLIC METHODS ####################
ICollisionComponent_Ptr Entity::collision_component()		{ return m_collisionComponent; }
IHealthComponent_Ptr Entity::health_component()				{ return m_healthComponent; }
int Entity::id() const										{ return m_id; }
ILookComponent_Ptr Entity::look_component()					{ return m_lookComponent; }
IPositionComponent_Ptr Entity::position_component()			{ return m_positionComponent; }
IVisibilityComponent_Ptr Entity::visibility_component()		{ return m_visibilityComponent; }
IYokeComponent_Ptr Entity::yoke_component()					{ return m_yokeComponent; }

void Entity::save(std::ostream& os) const
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

void Entity::set_collision_component(const ICollisionComponent_Ptr& collisionComponent)			{ m_collisionComponent = collisionComponent; }
void Entity::set_health_component(const IHealthComponent_Ptr& healthComponent)					{ m_healthComponent = healthComponent; }
void Entity::set_id(int id)																		{ m_id = id; }
void Entity::set_look_component(const ILookComponent_Ptr& lookComponent)						{ m_lookComponent = lookComponent; }
void Entity::set_position_component(const IPositionComponent_Ptr& positionComponent)			{ m_positionComponent = positionComponent; }
void Entity::set_visibility_component(const IVisibilityComponent_Ptr& visibilityComponent)		{ m_visibilityComponent = visibilityComponent; }
void Entity::set_yoke_component(const IYokeComponent_Ptr& yokeComponent)						{ m_yokeComponent = yokeComponent; }

}
