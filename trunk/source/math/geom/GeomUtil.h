/***
 * hesperus: GeomUtil.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_MATH_GEOM_GEOMUTIL
#define H_HESP_MATH_GEOM_GEOMUTIL

#include "Plane.h"
#include "Polygon.h"

namespace hesp {

//#################### CLASSES ####################
template <typename Vert, typename AuxData>
struct SplitResults
{
	typedef shared_ptr<Polygon<Vert,AuxData> > Polygon_Ptr;

	Polygon_Ptr back;
	Polygon_Ptr front;

	SplitResults(const Polygon_Ptr& back_, const Polygon_Ptr& front) : back(back_), front(front_) {}
};

//#################### GLOBAL FUNCTIONS ####################
PlaneClassifier classify_point_against_plane(const Vector3d& p, const Plane& plane);
template <typename Vert, typename AuxData> PlaneClassifier classify_polygon_against_plane(const Polygon<Vert,AuxData>& poly, const Plane& plane);
double distance_to_plane(const Vector3d& p, const Plane& plane);
template <typename Vert, typename AuxData> SplitResults<Vert,AuxData> split_polygon_across_plane(const Polygon<Vert,AuxData>& poly, const Plane& plane);

}

#include "GeomUtil.tpp"

#endif
