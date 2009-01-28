/***
 * hesperus: Entity.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Entity.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Entity::Entity(const std::string& entityClass)
:	m_id(-1), m_entityClass(entityClass)
{}

//#################### PUBLIC METHODS ####################
ICameraComponent_Ptr Entity::camera_component()				{ return m_cameraComponent; }
ICollisionComponent_Ptr Entity::collision_component()		{ return m_collisionComponent; }
const std::string& Entity::entity_class() const				{ return m_entityClass; }
IHealthComponent_Ptr Entity::health_component()				{ return m_healthComponent; }
int Entity::id() const										{ return m_id; }
#if 0
ILookComponent_Ptr Entity::look_component()					{ return m_lookComponent; }
IPositionComponent_Ptr Entity::position_component()			{ return m_positionComponent; }
#endif
IVisibilityComponent_Ptr Entity::visibility_component()		{ return m_visibilityComponent; }
IYokeComponent_Ptr Entity::yoke_component()					{ return m_yokeComponent; }

void Entity::save(std::ostream& os) const
{
	os << "Instance " << m_entityClass << '\n';
	os << "{\n";

	m_cameraComponent->save(os);
	m_collisionComponent->save(os);
	m_healthComponent->save(os);
#if 0
	m_lookComponent->save(os);
	m_positionComponent->save(os);
#endif
	m_visibilityComponent->save(os);

	os << "}\n";
}

void Entity::set_camera_component(const ICameraComponent_Ptr& cameraComponent)					{ m_cameraComponent = cameraComponent; }
void Entity::set_collision_component(const ICollisionComponent_Ptr& collisionComponent)			{ m_collisionComponent = collisionComponent; }
void Entity::set_health_component(const IHealthComponent_Ptr& healthComponent)					{ m_healthComponent = healthComponent; }
void Entity::set_id(int id)																		{ m_id = id; }
#if 0
void Entity::set_look_component(const ILookComponent_Ptr& lookComponent)						{ m_lookComponent = lookComponent; }
void Entity::set_position_component(const IPositionComponent_Ptr& positionComponent)			{ m_positionComponent = positionComponent; }
#endif
void Entity::set_visibility_component(const IVisibilityComponent_Ptr& visibilityComponent)		{ m_visibilityComponent = visibilityComponent; }
void Entity::set_yoke_component(const IYokeComponent_Ptr& yokeComponent)						{ m_yokeComponent = yokeComponent; }

}
