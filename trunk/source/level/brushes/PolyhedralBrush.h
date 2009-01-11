/***
 * hesperus: PolyhedralBrush.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_BRUSHES_POLYHEDRALBRUSH
#define H_HESP_LEVEL_BRUSHES_POLYHEDRALBRUSH

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/geom/AABB.h>
#include <source/math/geom/Polygon.h>

namespace hesp {

/**
This class template represents convex polyhedral brushes, in other words brushes
consisting of convex faces which completely enclose a convex volume.
*/
template <typename Poly>
class PolyhedralBrush
{
	//#################### TYPEDEFS ####################
private:
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	//#################### PRIVATE VARIABLES ####################
private:
	AABB3d m_bounds;
	PolyVector m_faces;

	//#################### CONSTRUCTORS ####################
public:
	explicit PolyhedralBrush(const AABB3d& bounds, const PolyVector& faces);

	//#################### PUBLIC METHODS ####################
public:
	const AABB3d& bounds() const;
	const PolyVector& faces() const;
};

}

#include "PolyhedralBrush.tpp"

#endif
