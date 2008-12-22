/***
 * hbsp: RenderingVector3d.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "RenderingVector3d.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
RenderingVector3d::RenderingVector3d() : x(0), y(0), z(0), u(0), v(0) {}

RenderingVector3d::RenderingVector3d(double x_, double y_, double z_, double u_, double v_)
:	x(x_), y(y_), z(z_), u(u_), v(v_)
{}

//#################### PUBLIC OPERATORS ####################
RenderingVector3d& RenderingVector3d::operator+=(const RenderingVector3d& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	u += rhs.u;
	v += rhs.v;
	return *this;
}

RenderingVector3d& RenderingVector3d::operator-=(const RenderingVector3d& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	u -= rhs.u;
	v -= rhs.v;
	return *this;
}

RenderingVector3d& RenderingVector3d::operator*=(double factor)
{
	x *= factor;
	y *= factor;
	z *= factor;
	u *= factor;
	v *= factor;
	return *this;
}

RenderingVector3d::operator Vector3d() const
{
	return Vector3d(x,y,z);
}

//#################### GLOBAL OPERATORS ####################
RenderingVector3d operator+(const RenderingVector3d& lhs, const RenderingVector3d& rhs)
{
	RenderingVector3d copy(lhs);
	copy += rhs;
	return copy;
}

RenderingVector3d operator-(const RenderingVector3d& lhs, const RenderingVector3d& rhs)
{
	RenderingVector3d copy(lhs);
	copy -= rhs;
	return copy;
}

RenderingVector3d operator*(double factor, const RenderingVector3d& v)
{
	RenderingVector3d copy(v);
	copy *= factor;
	return copy;
}

}
