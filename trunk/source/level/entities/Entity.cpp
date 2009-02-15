/***
 * hesperus: Entity.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Entity.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Entity::Entity(const std::string& entityType)
:	m_id(-1), m_entityType(entityType)
{}

//#################### PUBLIC METHODS ####################
ICameraComponent_Ptr Entity::camera_component()				{ return m_cameraComponent; }
ICollisionComponent_Ptr Entity::collision_component()		{ return m_collisionComponent; }
const std::string& Entity::entity_type() const				{ return m_entityType; }
IHealthComponent_Ptr Entity::health_component()				{ return m_healthComponent; }
int Entity::id() const										{ return m_id; }
INavComponent_Ptr Entity::nav_component()					{ return m_navComponent; }
IPhysicsComponent_Ptr Entity::physics_component()			{ return m_physicsComponent; }
IVisibilityComponent_Ptr Entity::visibility_component()		{ return m_visibilityComponent; }
IYokeComponent_Ptr Entity::yoke_component()					{ return m_yokeComponent; }

void Entity::save(std::ostream& os) const
{
	os << "Instance " << m_entityType << '\n';
	os << "{\n";

	if(m_cameraComponent) m_cameraComponent->save(os);
	if(m_collisionComponent) m_collisionComponent->save(os);
	if(m_healthComponent) m_healthComponent->save(os);
	if(m_physicsComponent) m_physicsComponent->save(os);
	if(m_visibilityComponent) m_visibilityComponent->save(os);
	if(m_yokeComponent) m_yokeComponent->save(os);

	os << "}\n";
}

void Entity::set_camera_component(const ICameraComponent_Ptr& cameraComponent)					{ m_cameraComponent = cameraComponent; }
void Entity::set_collision_component(const ICollisionComponent_Ptr& collisionComponent)			{ m_collisionComponent = collisionComponent; }
void Entity::set_health_component(const IHealthComponent_Ptr& healthComponent)					{ m_healthComponent = healthComponent; }
void Entity::set_id(int id)																		{ m_id = id; }
void Entity::set_nav_component(const INavComponent_Ptr& navComponent)							{ m_navComponent = navComponent; }
void Entity::set_physics_component(const IPhysicsComponent_Ptr& physicsComponent)				{ m_physicsComponent = physicsComponent; }
void Entity::set_visibility_component(const IVisibilityComponent_Ptr& visibilityComponent)		{ m_visibilityComponent = visibilityComponent; }
void Entity::set_yoke_component(const IYokeComponent_Ptr& yokeComponent)						{ m_yokeComponent = yokeComponent; }

}
