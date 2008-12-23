/***
 * hesperus: PortalGenerator.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
// TODO

//#################### PRIVATE METHODS ####################
/**
Finds the unique set of planes in which the specified polygons lie.
Note that planes which differ only in the orientation of their
normal are treated as equivalent here.

@param polygons	The polygons whose unique set of planes we wish to find
@return			As stated
*/
template <typename Vert, typename AuxData>
typename PortalGenerator::PlaneList_Ptr
PortalGenerator::find_unique_planes(const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons)
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	PlaneList_Ptr initialPlanes;

	for(PolyVector::const_iterator it=polygons.begin(), iend=polygons.end(); it!=iend; ++it)
	{
		Plane plane = make_plane(*it);

		const Vector3d& n = plane.normal();
		double d = plane.distance_value();

		// Standardize the plane representation so that there is a 1-1 mapping
		// between a unique plane and its representation. Thus, for example,
		// the plane (-1,0,0) . p = -1, or -x = -1, will be converted to being
		// (1,0,0) . p = 1, or x = 1. The planes are the same; what we are
		// discarding is the extra information we have about the plane normal.
		// TODO
	}

	// NYI
	throw 23;
}

}
