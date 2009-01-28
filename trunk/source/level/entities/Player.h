/***
 * hesperus: Player.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PLAYER
#define H_HESP_PLAYER

#include "BipedEntity.h"
#include "MortalEntity.h"

namespace hesp {

//#################### TYPEDEFS ####################
typedef shared_ptr<class Player> Player_Ptr;
typedef shared_ptr<const class Player> Player_CPtr;

/**
This class represents the player.
*/
class Player
:	public BipedEntity,
	public MortalEntity
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_health;
	Vector3d m_look;
	std::string m_modelFilename;

	//#################### CONSTRUCTORS ####################
public:
	Player(const std::vector<int>& aabbIndices,
		   const std::string& modelFilename,
		   int health,
		   const Vector3d& look,
		   int pose,
		   const Vector3d& position);

	//#################### PUBLIC METHODS ####################
public:
	int health() const;
	static Player_Ptr load(std::istream& is);
	const Vector3d& look() const;
	void save(std::ostream& os) const;
};

}

#endif
