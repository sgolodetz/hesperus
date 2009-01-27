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
				const std::string& modelFilename,
				const Vector3d& look,
				int pose,
				const Vector3d& position)
	:	CollidableEntity(position, aabbIndices, pose),
		OrientedEntity(look),
		VisibleEntity(modelFilename)
	{}

	//#################### PUBLIC METHODS ####################
public:
	BipedEntity *as_biped()
	{
		return this;
	}
};

}

#endif
