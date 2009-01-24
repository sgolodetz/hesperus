/***
 * hesperus: Player.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Player.h"

#include <source/io/FieldIO.h>

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

//#################### PUBLIC METHODS ####################
Player_Ptr Player::load(std::istream& is)
{
	std::vector<int> aabbIndices = FieldIO::read_intarray_field(is, "AABBs");
	std::string modelFilename = FieldIO::read_field(is, "GameModel");
	int health = FieldIO::read_typed_field<int>(is, "Health");
	int pose = FieldIO::read_typed_field<int>(is, "Pose");
	Vector3d position = FieldIO::read_typed_field<Vector3d>(is, "Position");

	LineIO::read_checked_line(is, "}");

	return Player_Ptr(new Player(aabbIndices, modelFilename, health, pose, position));
}

void Player::save(std::ostream& os) const
{
	os << "Instance Player\n";
	os << "{\n";

	// TODO

	os << "}\n";
}

}
