/***
 * hesperus: Entity.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITY
#define H_HESP_ENTITY

#include "ICameraComponent.h"
#include "ICollisionComponent.h"
#include "IHealthComponent.h"
#if 0
#include "ILookComponent.h"
#include "IPositionComponent.h"
#endif
#include "IVisibilityComponent.h"
#include "IYokeComponent.h"

namespace hesp {

class Entity
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_id;
	std::string m_entityClass;
	ICameraComponent_Ptr m_cameraComponent;
	ICollisionComponent_Ptr m_collisionComponent;
	IHealthComponent_Ptr m_healthComponent;
#if 0
	ILookComponent_Ptr m_lookComponent;
	IPositionComponent_Ptr m_positionComponent;
#endif
	IVisibilityComponent_Ptr m_visibilityComponent;
	IYokeComponent_Ptr m_yokeComponent;

	//#################### CONSTRUCTORS ####################
public:
	Entity(const std::string& entityClass);

	//#################### PUBLIC METHODS ####################
public:
	ICameraComponent_Ptr camera_component();
	ICollisionComponent_Ptr collision_component();
	const std::string& entity_class() const;
	IHealthComponent_Ptr health_component();
	int id() const;
#if 0
	ILookComponent_Ptr look_component();
	IPositionComponent_Ptr position_component();
#endif
	IVisibilityComponent_Ptr visibility_component();
	IYokeComponent_Ptr yoke_component();
	void save(std::ostream& os) const;
	void set_camera_component(const ICameraComponent_Ptr& cameraComponent);
	void set_collision_component(const ICollisionComponent_Ptr& collisionComponent);
	void set_health_component(const IHealthComponent_Ptr& healthComponent);
	void set_id(int id);
#if 0
	void set_look_component(const ILookComponent_Ptr& lookComponent);
	void set_position_component(const IPositionComponent_Ptr& positionComponent);
#endif
	void set_visibility_component(const IVisibilityComponent_Ptr& visibilityComponent);
	void set_yoke_component(const IYokeComponent_Ptr& yokeComponent);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Entity> Entity_Ptr;

}

#endif
