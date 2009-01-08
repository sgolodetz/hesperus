/***
 * hesperus: OnionPlane.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_ONIONBSP_ONIONPLANE
#define H_HESP_LEVEL_ONIONBSP_ONIONPLANE

#include <set>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/geom/Plane.h>

namespace hesp {

class OnionPlane
{
	//#################### PRIVATE VARIABLES ####################
private:
	Plane m_plane;
	Plane m_undirectedPlane;
	int m_mapIndex;

	//#################### CONSTRUCTORS ####################
public:
	OnionPlane(const Plane& plane, int mapIndex);

	//#################### PUBLIC METHODS ####################
public:
	int map_index() const;
	const Plane& plane() const;
	const Plane& undirected_plane() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<OnionPlane> OnionPlane_Ptr;
typedef shared_ptr<const OnionPlane> OnionPlane_CPtr;

}

#endif
