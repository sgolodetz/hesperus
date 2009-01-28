/***
 * hesperus: BipedEntity.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BIPEDENTITY
#define H_HESP_BIPEDENTITY

#include "CollidableEntity.h"
#include "OrientedEntity.h"
#include "VisibleEntity.h"
#include "YokeableEntity.h"

namespace hesp {

/**
This class represents a biped (i.e. two-legged) entity.
*/
class BipedEntity
:	public CollidableEntity,
	public OrientedEntity,
	public VisibleEntity,
	public YokeableEntity
{
	//#################### CONSTRUCTORS ####################
public:
	BipedEntity(const std::vector<int>& aabbIndices,
				int pose,
				const Vector3d& position)
	:	CollidableEntity(position, aabbIndices, pose)
	{}

	//#################### PUBLIC METHODS ####################
public:
	void walk(const Vector3d& dir, int milliseconds)
	{
		// TODO:	Collision handling (this should probably go in a separate function which takes
		//			a line segment from the current position to a desired goal position, and returns
		//			a valid location to which to actually move the biped)
		// TODO:	Use a proper biped walking speed

		const double BIPED_WALK_SPEED = 5.0;
		m_position += dir * BIPED_WALK_SPEED * (milliseconds/1000.0);
	}
};

}

#endif
