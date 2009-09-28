/***
 * hesperus: PointBounds.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PointBounds.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
double PointBounds::brush_expansion_distance(const Vector3d&) const
{
	return 0.0;
}

boost::optional<std::pair<Vector3d,Vector3d> > PointBounds::determine_halfray_intersection(const Vector3d& s, const Vector3d& v) const
{
	return boost::none;
}

double PointBounds::height() const
{
	return 0.0;
}

void PointBounds::render(const Vector3d& pos) const
{
	// No-op
}

}
