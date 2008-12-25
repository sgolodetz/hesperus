/***
 * hesperus: Antipenumbra.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSP_ANTIPENUMBRA
#define H_HESP_BSP_ANTIPENUMBRA

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/geom/GeomUtil.h>
#include <source/math/vectors/Vector3.h>
#include "Portal.h"

namespace hesp {

class Antipenumbra
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<Plane> m_planes;

	//#################### CONSTRUCTORS ####################
public:
	Antipenumbra(const Portal_Ptr& source, const Portal_Ptr& target);
	Antipenumbra(const Vector3d& source, const Portal_Ptr& target);

	//#################### PUBLIC METHODS ####################
public:
	template <typename Vert, typename AuxData>
	shared_ptr<Polygon<Vert,AuxData> > clip(const shared_ptr<Polygon<Vert,AuxData> >& poly);

	//#################### PRIVATE METHODS ####################
private:
	void add_clip_planes(const Portal_Ptr& from, const Portal_Ptr& to, PlaneClassifier desiredFromClassifier);
	static Plane_Ptr construct_clip_plane(const Vector3d& a, const Vector3d& b, const Vector3d& c);
};

}

#include "Antipenumbra.tpp"

#endif
