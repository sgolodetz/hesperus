/***
 * hesperus: Bone.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Bone.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Bone::Bone(const std::string& name, const Vector3d& position, const Vector3d& rotationAxis, double rotationAngle)
:	m_name(name)
{
	m_relativeMatrix = Matrix44::from_axis_angle_translation(rotationAxis, rotationAngle, position);

	// NYI
	throw 23;
}

}
