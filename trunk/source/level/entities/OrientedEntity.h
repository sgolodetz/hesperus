/***
 * hesperus: OrientedEntity.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ORIENTEDENTITY
#define H_HESP_ORIENTEDENTITY

#include <source/math/vectors/Vector3.h>
#include "Entity.h"

namespace hesp {

/**
This class represents an oriented entity (i.e. the entity has a look vector).
*/
class OrientedEntity : virtual public Entity
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~OrientedEntity() {}

	//#################### PUBLIC METHODS ####################
public:
	virtual const Vector3d& look() const = 0;
};

}

#endif
