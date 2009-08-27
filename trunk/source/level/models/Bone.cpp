/***
 * hesperus: Bone.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Bone.h"

#include <source/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Bone::Bone(const std::string& name, const Vector3d& position, const Vector3d& rotationAxis, double rotationAngle)
:	m_name(name), m_basePosition(position)
{
	m_baseRotation = RBTMatrix::from_axis_angle_translation(rotationAxis, rotationAngle, Vector3d(0,0,0));
}

//#################### PUBLIC METHODS ####################
RBTMatrix_Ptr& Bone::absolute_matrix()				{ return m_absoluteMatrix; }
const RBTMatrix_CPtr Bone::absolute_matrix() const	{ return m_absoluteMatrix; }
const Vector3d& Bone::base_position() const			{ return m_basePosition; }
const RBTMatrix_CPtr Bone::base_rotation() const	{ return m_baseRotation; }
const std::string& Bone::name() const				{ return m_name; }

Vector3d Bone::orientation() const
{
	if(!m_absoluteMatrix) throw Exception("The absolute matrix for the bone has not yet been calculated");

	// Extract the bone orientation from the absolute matrix and return it.
	RBTMatrix& m = *m_absoluteMatrix;
	return Vector3d(m(0,1), m(1,1), m(2,1));
}

Bone_Ptr Bone::parent()								{ return m_parent; }

Vector3d Bone::position() const
{
	if(!m_absoluteMatrix) throw Exception("The absolute matrix for the bone has not yet been calculated");

	// Extract the bone position from the absolute matrix and return it.
	RBTMatrix& m = *m_absoluteMatrix;
	return Vector3d(m(0,3), m(1,3), m(2,3));
}

RBTMatrix_Ptr& Bone::relative_matrix()				{ return m_relativeMatrix; }
const RBTMatrix_CPtr Bone::relative_matrix() const	{ return m_relativeMatrix; }
void Bone::set_parent(const Bone_Ptr& parent)		{ m_parent = parent; }

}
