/***
 * hesperus: GeomUtil.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_MATH_GEOM_GEOMUTIL
#define H_HESP_MATH_GEOM_GEOMUTIL

#include <utility>

#include "Plane.h"
#include "Polygon.h"

namespace hesp {

//#################### CLASSES ####################
template <typename Vert, typename AuxData>
struct SplitResults
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;

	Poly_Ptr back;
	Poly_Ptr front;

	SplitResults(const Poly_Ptr& back_, const Poly_Ptr& front_) : back(back_), front(front_) {}
};

//#################### GLOBAL FUNCTIONS ####################
PlaneClassifier classify_linesegment_against_plane(const Vector3d& p1, const Vector3d& p2, const Plane& plane, PlaneClassifier& cp1, PlaneClassifier& cp2);

PlaneClassifier classify_point_against_plane(const Vector3d& p, const Plane& plane);

template <typename Vert, typename AuxData>
PlaneClassifier classify_polygon_against_plane(const Polygon<Vert,AuxData>& poly, const Plane& plane);

template <typename Vec>
std::pair<Vec,double> determine_line_intersection_with_plane(const Vec& s, const Vec& v, const Plane& plane);

template <typename Vec>
std::pair<Vec,bool> determine_linesegment_intersection_with_plane(const Vec& p1, const Vec& p2, const Plane& plane);

double displacement_from_plane(const Vector3d& p, const Plane& plane);

double distance_to_plane(const Vector3d& p, const Plane& plane);

Vector3d generate_arbitrary_coplanar_unit_vector(const Plane& plane);

template <typename Vert, typename AuxData>
void load_polygons(std::istream& is, std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, int maxToRead = INT_MAX);

template <typename Vert, typename AuxData>
Plane make_plane(const Polygon<Vert,AuxData>& poly);

template <typename AuxData>
shared_ptr<Polygon<Vector3d,AuxData> > make_universe_polygon(const Plane& plane, const AuxData& auxData = AuxData());

Vector3d nearest_point_in_plane(const Vector3d& p, const Plane& plane);

template <typename Vert, typename AuxData>
bool point_in_polygon(const Vector3d& p, const Polygon<Vert,AuxData>& poly);

template <typename Vert, typename AuxData>
SplitResults<Vert,AuxData> split_polygon(const Polygon<Vert,AuxData>& poly, const Plane& plane);

template <typename Vert, typename AuxData>
void write_polygons(std::ostream& os, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons);

//################## HELPER METHODS FOR THE split_polygon FUNCTION ##################
int next_vert(int index, int vertCount);

int prev_vert(int index, int vertCount);

}

#include "GeomUtil.tpp"

#endif
