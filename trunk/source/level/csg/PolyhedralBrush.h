/***
 * hesperus: PolyhedralBrush.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_CSG_POLYHEDRALBRUSH
#define H_HESP_LEVEL_CSG_POLYHEDRALBRUSH

#include <ostream>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/geom/AABB.h>
#include <source/math/geom/Polygon.h>

namespace hesp {

/**
This class represents convex polyhedral brushes, in other words brushes consisting of convex faces which
completely enclose a convex volume.
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

//#################### GLOBAL OPERATORS ####################
template <typename Poly>
std::ostream& operator<<(std::ostream& os, const PolyhedralBrush<Poly>& rhs);

}

#include "PolyhedralBrush.tpp"

#endif
