/***
 * hesperus: OnionPlane.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "OnionPlane.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
OnionPlane::OnionPlane(const Plane& plane, int mapIndex)
:	m_plane(plane), m_undirectedPlane(plane.to_undirected_form()), m_mapIndex(mapIndex)
{}

//#################### PUBLIC METHODS ####################
int OnionPlane::map_index() const
{
	return m_mapIndex;
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
