/***
 * hesperus: RBTQuaternion.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "RBTQuaternion.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
RBTQuaternion::RBTQuaternion(const Quaternion& rotation, const Vector3d& translation)
:	m_rotation(rotation), m_translation(translation)
{}

//#################### STATIC FACTORY METHODS ####################
RBTQuaternion_Ptr RBTQuaternion::from_axis_angle_translation(Vector3d axis, double angle, const Vector3d& translation)
{
	return RBTQuaternion_Ptr(new RBTQuaternion(Quaternion::from_axis_angle(axis, angle), translation));
}

//#################### PUBLIC METHODS ####################
Vector3d RBTQuaternion::apply(const Vector3d& p) const
{
	return m_rotation.apply_unit_rotation(p) + m_translation;
}

RBTQuaternion_Ptr RBTQuaternion::interpolate(const RBTQuaternion_Ptr& lhs, const RBTQuaternion_Ptr& rhs, double t)
{
	Quaternion newRot = Quaternion::slerp(lhs->rotation(), rhs->rotation(), t);
	Vector3d newTrans = (1-t)*lhs->translation() + t*rhs->translation();
	return RBTQuaternion_Ptr(new RBTQuaternion(newRot, newTrans));
}

const Quaternion& RBTQuaternion::rotation() const
{
	return m_rotation;
}

const Vector3d& RBTQuaternion::translation() const
{
	return m_translation;
}

}
