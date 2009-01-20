/***
 * hesperus: Player.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PLAYER
#define H_HESP_PLAYER

#include "CollidableEntity.h"
#include "MortalEntity.h"
#include "VisibleEntity.h"

namespace hesp {

/**
This class represents the player.
*/
class Player
:	virtual public CollidableEntity,
	virtual public MortalEntity,
	virtual public VisibleEntity		// in third-person view
{
	// TODO
};

}

#endif
