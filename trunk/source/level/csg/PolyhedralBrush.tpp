/***
 * hesperus: PolyhedralBrush.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

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

}
