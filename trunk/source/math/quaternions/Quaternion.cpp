/***
 * hesperus: Quaternion.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Quaternion.h"

#include <cmath>

#include <source/exceptions/Exception.h>
#include <source/math/Constants.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Quaternion::Quaternion(double w_, double x_, double y_, double z_)
:	w(w_), x(x_), y(y_), z(z_)
{}

//#################### PUBLIC OPERATORS ####################
Quaternion& Quaternion::operator+=(const Quaternion& rhs)
{
	w += rhs.w; x += rhs.x; y += rhs.y; z += rhs.z;
	return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs)
{
	// Note:	I've implemented *= in terms of * rather than the other way round
	//			because each of the new values uses many of the old values here,
	//			so we'd otherwise have to cache them.
	*this = *this * rhs;
	return *this;
}

Quaternion& Quaternion::operator*=(double scale)
{
	w *= scale; x *= scale; y *= scale; z *= scale;
	return *this;
}

//#################### PUBLIC METHODS ####################
Vector3d Quaternion::apply_rotation(const Vector3d& p) const
{
	Quaternion quatP(0, p.x, p.y, p.z);
	Quaternion result = *this * quatP * inverse();
	return Vector3d(result.x, result.y, result.z);
}

Quaternion Quaternion::conjugate() const
{
	return Quaternion(w, -x, -y, -z);
}

double Quaternion::dot(const Quaternion& rhs) const
{
	return w*rhs.w + x*rhs.x + y*rhs.y + z*rhs.z;
}

Quaternion Quaternion::inverse() const
{
	double k = length_squared();
	return Quaternion(w/k, -x/k, -y/k, -z/k);
}

double Quaternion::length() const
{
	return sqrt(w*w + x*x + y*y + z*z);
}

double Quaternion::length_squared() const
{
	return w*w + x*x + y*y + z*z;
}

Quaternion Quaternion::lerp(const Quaternion& q1, const Quaternion& q2, double t)
{
	return Quaternion(
		(1-t)*q1.w + t*q2.w,
		(1-t)*q1.x + t*q2.x,
		(1-t)*q1.y + t*q2.y,
		(1-t)*q1.z + t*q2.z
	);
}

Quaternion& Quaternion::normalize()
{
	double len = length();
	if(len < SMALL_EPSILON) throw Exception("Unable to normalize quaternion: too close to zero");
	return (*this) *= 1/len;
}

Quaternion Quaternion::slerp(const Quaternion& q1, const Quaternion& q2, double t)
{
	assert(fabs(q1.length_squared() - 1) < SMALL_EPSILON);
	assert(fabs(q2.length_squared() - 1) < SMALL_EPSILON);

	if(t < 0) return q1;
	if(t > 1) return q2;

	// Calculate the dot product between the quaternions. If it's < 0, then
	// negate either one of them (q2 was arbitrarily picked here). This is
	// valid since q and -q represent the same rotation. Choosing the signs
	// so that q1 . q2 >= 0 is a good idea because it ensures that the
	// interpolation takes place over the shortest path.
	Quaternion q3 = q2;
	double dotProd = q1.dot(q3);
	if(dotProd < 0)
	{
		q3 *= -1;
		dotProd *= -1;
	}

	if(fabs(dotProd - 1) > EPSILON)
	{
		double theta = acos(dotProd);
		double sinTheta = sin(theta);

		Quaternion ret = sin(theta*(1-t)) * q1;
		ret += sin(theta*t) * q2;
		ret *= 1/sinTheta;

		return ret;
	}
	else
	{
		// If the angle theta between q1 and q3 is pretty small, we can't use
		// slerp since dividing by sin theta would give us a divide-by-zero
		// error, so we fall back to using simple lerp, which works fine here.
		// Note that the other special case (where theta is nearly pi) doesn't
		// occur because of the sign flip we do to q2 in this case above.
		Quaternion ret = lerp(q1, q3, t);
		ret.normalize();
		return ret;
	}
}

//#################### GLOBAL OPERATORS ####################
Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
{
	return Quaternion(
		q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z,
		q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
		q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x,
		q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w
	);
}

Quaternion operator*(const Quaternion& q, double scale)
{
	Quaternion copy(q);
	copy *= scale;
	return copy;
}

Quaternion operator*(double scale, const Quaternion& q)
{
	Quaternion copy(q);
	copy *= scale;
	return copy;
}

std::ostream& operator<<(std::ostream& os, const Quaternion& q)
{
	os << "( " << q.w << ' ' << q.x << ' ' << q.y << ' ' << q.z << " )";
	return os;
}

}
