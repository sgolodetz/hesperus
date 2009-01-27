/***
 * hesperus: Guard.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Guard.h"

#include <source/io/FieldIO.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Guard::Guard(const std::vector<int>& aabbIndices,
			 const std::string& modelFilename,
			 int health,
			 const Vector3d& look,
			 int pose,
			 const Vector3d& position)
:	BipedEntity(aabbIndices, modelFilename, look, pose, position),
	MortalEntity(health)
{}

//#################### PUBLIC METHODS ####################
Guard *Guard::as_guard()
{
	return this;
}

Guard_Ptr Guard::load(std::istream& is)
{
	std::vector<int> aabbIndices = FieldIO::read_intarray_field(is, "AABBs");
	std::string modelFilename = FieldIO::read_field(is, "GameModel");
	int health = FieldIO::read_typed_field<int>(is, "Health");
	Vector3d look = FieldIO::read_typed_field<Vector3d>(is, "Look");
	int pose = FieldIO::read_typed_field<int>(is, "Pose");
	Vector3d position = FieldIO::read_typed_field<Vector3d>(is, "Position");

	LineIO::read_checked_line(is, "}");

	return Guard_Ptr(new Guard(aabbIndices, modelFilename, health, look, pose, position));
}

void Guard::save(std::ostream& os) const
{
	os << "Instance Guard\n";
	os << "{\n";

	FieldIO::write_intarray_field(os, "AABBs", m_aabbIndices);
	FieldIO::write_typed_field(os, "GameModel", m_modelFilename);
	FieldIO::write_typed_field(os, "Health", m_health);
	FieldIO::write_typed_field(os, "Look", m_look);
	FieldIO::write_typed_field(os, "Pose", m_pose);
	FieldIO::write_typed_field(os, "Position", m_position);

	os << "}\n";
}

}
