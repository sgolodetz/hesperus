/***
 * hesperus: Player.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Player.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Player::Player(const std::vector<int>& aabbIndices,
			   const std::string& modelFilename,
			   int health,
			   int pose,
			   const Vector3d& position)
:	CollidableEntity(position, aabbIndices, pose),
	MortalEntity(health),
	VisibleEntity(/*modelFilename*/)
{}

}
