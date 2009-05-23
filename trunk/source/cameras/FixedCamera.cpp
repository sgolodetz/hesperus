/***
 * hesperus: FixedCamera.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FixedCamera.h"

#include <source/ogl/WrappedGL.h>
#include <gl/glu.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
FixedCamera::FixedCamera(const Vector3d& eye, const Vector3d& look, const Vector3d& up)
:	m_eye(eye), m_at(eye + look), m_up(up)
{}

//#################### PUBLIC METHODS ####################
bool FixedCamera::is_inside_viewer() const
{
	return false;
}

Vector3d FixedCamera::set_view() const
{
	gluLookAt(m_eye.x, m_eye.y, m_eye.z, m_at.x, m_at.y, m_at.z, m_up.x, m_up.y, m_up.z);
	return m_eye;
}

}
