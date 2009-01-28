/***
 * hesperus: VariableCamera.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "VariableCamera.h"

#include <source/ogl/WrappedGL.h>
#include <gl/glu.h>

#include <source/math/Constants.h>
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
	// Note 1:	We try and optimise things a little by observing that there's no point rotating an
	//			axis about itself and that generally when we rotate about an axis, we'll be passing
	//			it in as the parameter axis, e.g. camera.rotate(camera.n(), Math.PI/2).

	// Note 2:	We prevent the camera's new n vector from being directly upwards or downwards.

	// Note 3:	We prevent the camera's new v vector from pointing below the horizontal.

	Vector3d newN = m_nVector;
	if(&axis != &m_nVector)
	{
		newN = rotate_about_axis(m_nVector, angle, axis);
		if(newN.cross(Vector3d(0,0,1)).length() < EPSILON) return;
	}

	Vector3d newV = m_vVector;
	if(&axis != &m_vVector)
	{
		newV = rotate_about_axis(m_vVector, angle, axis);
		if(newV.z < 0) return;
	}

	m_nVector = newN;
	m_vVector = newV;

	if(&axis != &m_uVector) m_uVector = rotate_about_axis(m_uVector, angle, axis);
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
