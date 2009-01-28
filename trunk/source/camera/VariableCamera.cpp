/***
 * hesperus: VariableCamera.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "VariableCamera.h"

#include <source/ogl/WrappedGL.h>
#include <gl/glu.h>

#include <source/math/vectors/VectorUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
/**
Constructs an upright (i.e. non-skewed) camera with the specified position and look vectors.

Preconditions:

- look must not point in the direction (0,0,1)
*/
VariableCamera::VariableCamera(const Vector3d& position, const Vector3d& look)
:	m_position(position), m_nVector(look)
{
	Vector3d up(0,0,1);
	m_nVector.normalize();
	m_uVector = up.cross(m_nVector).normalize();
	m_vVector = m_nVector.cross(m_uVector).normalize();
}

//#################### PUBLIC METHODS ####################
void VariableCamera::move_n(double delta)
{
	m_position += delta*m_nVector;
}

void VariableCamera::move_u(double delta)
{
	m_position += delta*m_uVector;
}

void VariableCamera::move_v(double delta)
{
	m_position += delta*m_vVector;
}

const Vector3d& VariableCamera::n() const
{
	return m_nVector;
}

const Vector3d& VariableCamera::position() const
{
	return m_position;
}

void VariableCamera::rotate(const Vector3d& axis, double angle)
{
	// Note: We try and optimise things a little by observing that there's no point rotating
	// an axis about itself and that generally when we rotate about an axis, we'll be passing
	// it in as the parameter axis, e.g. camera.rotate(camera.n(), Math.PI/2).
	if(&axis != &m_nVector) m_nVector = rotate_about_axis(m_nVector, angle, axis);
	if(&axis != &m_uVector) m_uVector = rotate_about_axis(m_uVector, angle, axis);
	if(&axis != &m_vVector) m_vVector = rotate_about_axis(m_vVector, angle, axis);
}

void VariableCamera::set_position(const Vector3d& position)
{
	m_position = position;
}

const Vector3d& VariableCamera::u() const
{
	return m_uVector;
}

const Vector3d& VariableCamera::v() const
{
	return m_vVector;
}

}
