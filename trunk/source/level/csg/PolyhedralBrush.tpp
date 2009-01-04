/***
 * hesperus: PolyhedralBrush.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
template <typename Vert, typename AuxData>
PolyhedralBrush<Vert,AuxData>::PolyhedralBrush(const AABB3d& bounds, const PolyVector& faces)
:	m_bounds(bounds), m_faces(faces)
{}

//#################### PUBLIC METHODS ####################
template <typename Vert, typename AuxData>
const AABB3d& PolyhedralBrush<Vert,AuxData>::bounds() const
{
	return m_bounds;
}

template <typename Vert, typename AuxData>
const typename PolyhedralBrush<Vert,AuxData>::PolyVector&
PolyhedralBrush<Vert,AuxData>::faces() const
{
	return m_faces;
}

//#################### GLOBAL OPERATORS ####################
template <typename Vert, typename AuxData>
std::ostream& operator<<(std::ostream& os, const PolyhedralBrush<Vert,AuxData>& rhs)
{
	os << "{\n";
	os << rhs.bounds().minimum() << ' ' << rhs.bounds().maximum() << '\n';
	write_polygons(os, rhs.faces());
	os << "}\n";
	return os;
}

}
