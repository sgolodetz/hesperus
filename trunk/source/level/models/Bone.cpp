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
}

//#################### PUBLIC METHODS ####################
const std::string& Bone::name() const
{
	return m_name;
}

const Matrix44_Ptr& Bone::relative_matrix() const
{
	return m_relativeMatrix;
}

void Bone::set_parent(const Bone_Ptr& parent)
{
	m_parent = parent;
}

}
