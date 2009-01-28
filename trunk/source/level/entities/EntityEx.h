/***
 * hesperus: EntityEx.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITYEX
#define H_HESP_ENTITYEX

#include "ICollisionComponent.h"
#include "IHealthComponent.h"
#include "ILookComponent.h"
#include "IPositionComponent.h"
#include "IVisibilityComponent.h"
#include "IYokeComponent.h"

namespace hesp {

class EntityEx
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_id;
	std::string m_archetype;
	ICollisionComponent_Ptr m_collisionComponent;
	IHealthComponent_Ptr m_healthComponent;
	ILookComponent_Ptr m_lookComponent;
	IPositionComponent_Ptr m_positionComponent;
	IVisibilityComponent_Ptr m_visibilityComponent;
	IYokeComponent_Ptr m_yokeComponent;

	//#################### CONSTRUCTORS ####################
public:
	EntityEx(const std::string& archetype);

	//#################### PUBLIC METHODS ####################
public:
	int id() const;
	void save(std::ostream& os) const;
	void set_id(int id);
};

}

#endif
