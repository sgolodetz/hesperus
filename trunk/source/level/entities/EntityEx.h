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
	ICollisionComponent_Ptr m_collisionComponent;
	IHealthComponent_Ptr m_healthComponent;
	ILookComponent_Ptr m_lookComponent;
	IPositionComponent_Ptr m_positionComponent;
	IVisibilityComponent_Ptr m_visibilityComponent;
	IYokeComponent_Ptr m_yokeComponent;

	//#################### CONSTRUCTORS ####################
public:
	EntityEx() : m_id(-1) {}

	//#################### PUBLIC METHODS ####################
public:
	int id() const
	{
		return m_id;
	}

	void save(std::ostream& os) const
	{
		// TODO
	}

	void set_id(int id)
	{
		m_id = id;
	}
};

}

#endif
