/***
 * hesperus: Camera.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Camera.h"

#include <source/ogl/WrappedGL.h>
#include <gl/glu.h>

#include <source/math/vectors/VectorUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Camera::Camera(const Vector3d& position, const Vector3d& look, const Vector3d& up)
:	m_position(position), m_nVector(look), m_vVector(up)
{
	m_nVector.normalize();
	m_vVector.normalize();
	m_uVector = m_vVector.cross(m_nVector).normalize();
}

//#################### PUBLIC METHODS ####################
void Camera::draw_axes() const
{
	// TODO
}

void Camera::move_n(double delta)
{
	m_position += delta*m_nVector;
}

void Camera::move_u(double delta)
{
	m_position += delta*m_uVector;
}

void Camera::move_v(double delta)
{
	m_position += delta*m_vVector;
}

const Vector3d& Camera::n() const
{
	return m_nVector;
}

const Vector3d& Camera::position() const
{
	return m_position;
}

void Camera::rotate(const Vector3d& axis, double angle)
{
	// Note: We try and optimise things a little by observing that there's no point rotating
	// an axis about itself and that generally when we rotate about an axis, we'll be passing
	// it in as the parameter axis, e.g. camera.rotate(camera.n(), Math.PI/2).
	if(&axis != &m_nVector) m_nVector = rotate_about_axis(m_nVector, angle, axis);
	if(&axis != &m_uVector) m_uVector = rotate_about_axis(m_uVector, angle, axis);
	if(&axis != &m_vVector) m_vVector = rotate_about_axis(m_vVector, angle, axis);
}

void Camera::set_position(const Vector3d& position)
{
	m_position = position;
}

void Camera::set_view() const
{
	glLoadIdentity();
	gluLookAt(	m_position.x, m_position.y, m_position.z,
				m_position.x + m_nVector.x, m_position.y + m_nVector.y, m_position.z + m_nVector.z,
				m_vVector.x, m_vVector.y, m_vVector.z);
}

const Vector3d& Camera::u() const
{
	return m_uVector;
}

const Vector3d& Camera::v() const
{
	return m_vVector;
}

}
