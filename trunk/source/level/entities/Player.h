/***
 * hesperus: Player.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PLAYER
#define H_HESP_PLAYER

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "CollidableEntity.h"
#include "MortalEntity.h"
#include "VisibleEntity.h"

namespace hesp {

/**
This class represents the player.
*/
class Player
:	public CollidableEntity,
	public MortalEntity,
	public VisibleEntity		// in third-person view
{
	//#################### CONSTRUCTORS ####################
public:
	Player(const std::vector<int>& aabbIndices,
		   const std::string& modelFilename,
		   int health,
		   int pose,
		   const Vector3d& position);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Player> Player_Ptr;
typedef shared_ptr<const Player> Player_CPtr;

}

#endif
