/***
 * hesperus: Entity.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITY
#define H_HESP_ENTITY

#include "ICameraComponent.h"
#include "ICollisionComponent.h"
#include "IHealthComponent.h"
#include "IPhysicsComponent.h"
#include "INavComponent.h"
#include "IVisibilityComponent.h"
#include "IYokeComponent.h"

namespace hesp {

class Entity
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_id;
	std::string m_entityType;
	ICameraComponent_Ptr m_cameraComponent;
	ICollisionComponent_Ptr m_collisionComponent;
	IHealthComponent_Ptr m_healthComponent;
	INavComponent_Ptr m_navComponent;
	IPhysicsComponent_Ptr m_physicsComponent;
	IVisibilityComponent_Ptr m_visibilityComponent;
	IYokeComponent_Ptr m_yokeComponent;

	//#################### CONSTRUCTORS ####################
public:
	Entity(const std::string& entityType);

	//#################### PUBLIC METHODS ####################
public:
	ICameraComponent_Ptr camera_component();
	ICollisionComponent_Ptr collision_component();
	const std::string& entity_type() const;
	IHealthComponent_Ptr health_component();
	int id() const;
	INavComponent_Ptr nav_component();
	IPhysicsComponent_Ptr physics_component();
	IVisibilityComponent_Ptr visibility_component();
	IYokeComponent_Ptr yoke_component();
	void save(std::ostream& os) const;
	void set_camera_component(const ICameraComponent_Ptr& cameraComponent);
	void set_collision_component(const ICollisionComponent_Ptr& collisionComponent);
	void set_health_component(const IHealthComponent_Ptr& healthComponent);
	void set_id(int id);
	void set_nav_component(const INavComponent_Ptr& navComponent);
	void set_physics_component(const IPhysicsComponent_Ptr& physicsComponent);
	void set_visibility_component(const IVisibilityComponent_Ptr& visibilityComponent);
	void set_yoke_component(const IYokeComponent_Ptr& yokeComponent);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Entity> Entity_Ptr;

}

#endif
