/***
 * hesperus: BipedEntity.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BIPEDENTITY
#define H_HESP_BIPEDENTITY

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "CollidableEntity.h"
#include "OrientedEntity.h"
#include "VisibleEntity.h"

namespace hesp {

/**
This class represents a biped (i.e. two-legged) entity.
*/
class BipedEntity
:	public CollidableEntity,
	public OrientedEntity,
	public VisibleEntity
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
	// TODO: attach_yoke(BipedYoke)
};

//#################### TYPEDEFS ####################
typedef shared_ptr<BipedEntity> BipedEntity_Ptr;

}

#endif
