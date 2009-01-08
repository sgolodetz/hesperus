/***
 * hesperus: OnionPlane.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "OnionPlane.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
OnionPlane::OnionPlane(const Plane& plane, int mapIndex)
:	m_plane(plane), m_undirectedPlane(plane.to_undirected_form())
{
	add_map_index(mapIndex);
}

//#################### PUBLIC METHODS ####################
void OnionPlane::add_map_index(int mapIndex)
{
	m_mapIndices.insert(mapIndex);
}

const std::set<int>& OnionPlane::map_indices() const
{
	return m_mapIndices;
}

const Plane& OnionPlane::plane() const
{
	return m_plane;
}

const Plane& OnionPlane::undirected_plane() const
{
	return m_undirectedPlane;
}

}
