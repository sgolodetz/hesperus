/***
 * hesperus: CollidableEntity.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_COLLIDABLEENTITY
#define H_HESP_COLLIDABLEENTITY

#include "LocatableEntity.h"

namespace hesp {

/**
This class represents a collidable entity (i.e. other things can collide
with this entity, so it maintains appropriate collision AABBs).
*/
class CollidableEntity : virtual public LocatableEntity
{
	// TODO
};

}

#endif
