/***
 * hesperus: PolyhedralBrush.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
template <typename Poly>
PolyhedralBrush<Poly>::PolyhedralBrush(const AABB3d& bounds, const PolyVector& faces)
:	m_bounds(bounds), m_faces(faces)
{}

//#################### PUBLIC METHODS ####################
template <typename Poly>
const AABB3d& PolyhedralBrush<Poly>::bounds() const
{
	return m_bounds;
}

template <typename Poly>
const typename PolyhedralBrush<Poly>::PolyVector&
PolyhedralBrush<Poly>::faces() const
{
	return m_faces;
}

//#################### GLOBAL OPERATORS ####################
template <typename Poly>
std::ostream& operator<<(std::ostream& os, const PolyhedralBrush<Poly>& rhs)
{
	os << "{\n";
	os << rhs.bounds().minimum() << ' ' << rhs.bounds().maximum() << '\n';
	write_polygons(os, rhs.faces(), true);
	os << "}\n";
	return os;
}

}
