/***
 * hesperus: Bone.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Bone.h"

#include <source/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Bone::Bone(const std::string& name, const Vector3d& position, const Vector3d& rotationAxis, double rotationAngle)
:	m_name(name)
{
	m_relativeMatrix = Matrix44::from_axis_angle_translation(rotationAxis, rotationAngle, position);
}

//#################### PUBLIC METHODS ####################
Matrix44_Ptr& Bone::absolute_matrix()				{ return m_absoluteMatrix; }
const Matrix44_Ptr& Bone::absolute_matrix() const	{ return m_absoluteMatrix; }
const std::string& Bone::name() const				{ return m_name; }
const Bone_Ptr& Bone::parent() const				{ return m_parent; }

Vector3d Bone::position() const
{
	if(!m_absoluteMatrix) throw Exception("The absolute matrix for the bone has not yet been calculated");

	// Extract the bone position from the absolute matrix and return it.
	Matrix44& m = *m_absoluteMatrix;
	return Vector3d(m(0,3), m(1,3), m(2,3));
}

Matrix44_Ptr& Bone::relative_matrix()				{ return m_relativeMatrix; }
const Matrix44_Ptr& Bone::relative_matrix() const	{ return m_relativeMatrix; }
void Bone::set_parent(const Bone_Ptr& parent)		{ m_parent = parent; }

}
