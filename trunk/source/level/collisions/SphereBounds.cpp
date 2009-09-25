/***
 * hesperus: SphereBounds.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "SphereBounds.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
SphereBounds::SphereBounds(double radius)
:	m_radius(radius)
{}

//#################### PUBLIC METHODS ####################
double SphereBounds::brush_expansion_distance(const Vector3d&) const
{
	return m_radius;
}

boost::optional<std::pair<Vector3d,Vector3d> > SphereBounds::determine_halfray_intersection(const Vector3d& s, const Vector3d& v) const
{
	return boost::none;

	// NYI
	throw 23;
}

double SphereBounds::height() const
{
	return m_radius * 2;
}

void SphereBounds::render(const Vector3d& pos) const
{
	// NYI
	throw 23;
}

}
