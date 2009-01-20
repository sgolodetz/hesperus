/***
 * hesperus: Player.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PLAYER
#define H_HESP_PLAYER

#include "CollidableEntity.h"
#include "LocatableEntity.h"
#include "MortalEntity.h"
#include "VisibleEntity.h"

namespace hesp {

/**
This class represents the player.
*/
class Player
:	public CollidableEntity,
	public LocatableEntity,
	public MortalEntity,
	public VisibleEntity		// in third-person view
{
	// TODO
};

}

#endif
