/***
 * hesperus: Plane.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Plane.h"

#include <cmath>

#include <source/exceptions/InvalidParameterException.h>
#include <source/math/Constants.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
/**
Constructs a new plane representing n . x - d = 0 from n and d.

@param n	The plane normal
@param d	The plane distance value
@throws InvalidParameterException	If |n| <= SMALL_EPSILON
*/
Plane::Plane(const Vector3d& n, double d)
:	m_n(n), m_d(d)
{
	ensure_invariant();
}

/**
Constructs a new plane representing n . x - d = 0 from n and x.

@param n	The plane normal
@param d	An arbitrary point on the plane
@throws InvalidParameterException	If |n| <= SMALL_EPSILON
*/
Plane::Plane(const Vector3d& n, const Vector3d& x)
:	m_n(n), m_d(n.dot(x))
{
	ensure_invariant();
}

//#################### PUBLIC METHODS ####################
/**
Classifies the point p against the plane.

@param p	The point to be classified
@return		CP_BACK, if the point is behind the plane; CP_COPLANAR, if it is on the plane; CP_FRONT, if it is in front of the plane
*/
Plane::CP Plane::classify_point(const Vector3d& p) const
{
	double value = m_n.dot(p) - m_d;

	if(fabs(value) < EPSILON) return CP_COPLANAR;
	else if(value > 0) return CP_FRONT;
	else return CP_BACK;
}

/**
Calculates the perpendicular distance between the plane and the point p.

@param p	The point whose perpendicular distance from the plane we wish to determine
@return		The perpendicular distance between p and the plane
*/
double Plane::distance_to_point(const Vector3d& p) const
{
	// Note that this equation is valid precisely because the stored normal is unit length.
	return fabs(m_n.dot(p) - m_d);
}

/**
Returns d, the plane distance value.

@return	As stated
*/
double Plane::distance_value() const
{
	return m_d;
}

/**
Returns n, the plane normal.

@return	As stated
*/
const Vector3d& Plane::normal() const
{
	return m_n;
}

//#################### PRIVATE METHODS ####################
void Plane::ensure_invariant()
{
	double length = m_n.length();
	if(length > SMALL_EPSILON)
	{
		m_n /= length;
		m_d /= length;
	}
	else throw InvalidParameterException("The plane's normal must be non-zero");
}

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, Plane::CP rhs)
{
	switch(rhs)
	{
		case Plane::CP_BACK:
			os << "CP_BACK";
			break;
		case Plane::CP_COPLANAR:
			os << "CP_COPLANAR";
			break;
		case Plane::CP_FRONT:
			os << "CP_FRONT";
			break;
		case Plane::CP_STRADDLE:
			os << "CP_STRADDLE";
			break;
	}
	return os;
}

}
