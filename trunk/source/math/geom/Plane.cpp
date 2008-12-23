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

/**
Returns a version of the plane ax + by + cz - d = 0 such that the
first one of a, b, c to be non-zero is positive. For example, this
would return 0x+1y-1z-23=0 for both 0x+1y-1z-23=0 and 0x-1y+1z+23=0.
*/
Plane Plane::to_undirected_form() const
{
	double x = m_n.x, y = m_n.y, z = m_n.z, d = m_d;

	if(fabs(x) < EPSILON) x = 0;
	if(fabs(y) < EPSILON) y = 0;
	if(fabs(z) < EPSILON) z = 0;

	bool zeroX = x == 0;
	bool zeroY = y == 0;

	bool flip = (x < 0) ||
				(zeroX && y < 0) ||
				(zeroX && zeroY && z < 0);

	if(flip) return Plane(Vector3d(-x,-y,-z),-d);
	else return Plane(Vector3d(x,y,z),d);
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
std::ostream& operator<<(std::ostream& os, PlaneClassifier rhs)
{
	switch(rhs)
	{
		case CP_BACK:
			os << "CP_BACK";
			break;
		case CP_COPLANAR:
			os << "CP_COPLANAR";
			break;
		case CP_FRONT:
			os << "CP_FRONT";
			break;
		case CP_STRADDLE:
			os << "CP_STRADDLE";
			break;
	}
	return os;
}

}
