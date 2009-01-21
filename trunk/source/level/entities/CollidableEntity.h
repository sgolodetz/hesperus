/***
 * hesperus: CollidableEntity.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_COLLIDABLEENTITY
#define H_HESP_COLLIDABLEENTITY

#include <vector>

#include <source/math/geom/AABB.h>
#include "LocatableEntity.h"

namespace hesp {

/**
This class represents a collidable entity (i.e. other things can collide
with this entity, so it maintains appropriate collision AABBs).
*/
class CollidableEntity : virtual public LocatableEntity
{
	//#################### PROTECTED VARIABLES ####################
protected:
	std::vector<int> m_aabbIndices;

	//#################### CONSTRUCTORS ####################
public:
	CollidableEntity(const Vector3d& position, const std::vector<int>& aabbIndices)
	:	LocatableEntity(position), m_aabbIndices(aabbIndices)
	{}

	//#################### PUBLIC METHODS ####################
public:
	const std::vector<int>& aabb_indices() const
	{
		return m_aabbIndices;
	}
};

}

#endif
