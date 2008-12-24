/***
 * hesperus: PortalGenerator.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <set>

#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
template <typename Vert, typename AuxData>
typename PortalGenerator::PortalList_Ptr
PortalGenerator::generate_portals(const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, const BSPTree_Ptr& tree)
{
	PortalList_Ptr portals(new PortalList);
	PlaneList_Ptr planes = find_unique_planes(polygons);

	for(PlaneList::const_iterator it=planes->begin(), iend=planes->end(); it!=iend; ++it)
	{
		Portal_Ptr portal = make_initial_portal(*it);
		portals->splice(portals->end(), clip_portal_to_tree(portal, tree));
	}

	// Generate the opposite-facing portals.
	for(PortalList::iterator it=portals->begin(), iend=portals->end(); it!=iend; ++it)
	{
		Portal_Ptr portal = *it;

		// Construct the reverse portal.
		Portal_Ptr reversePortal(portal->flipped_winding());
		const LeafLink& leafLink = portal->auxiliary_data();
		reversePortal->auxiliary_data() = LeafLink(leafLink.toLeaf, leafLink.fromLeaf);

		// Insert it after the existing portal in the list.
		++it;
		it = portals->insert(it, reversePortal);
	}

	return portals;
}

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

	const double angleTolerance = 0.5 * PI / 180;	// convert 0.5 degrees to radians
	const double distTolerance = 0.001;
	std::set<Plane, PlanePred> planes(PlanePred(angleTolerance, distTolerance));

	for(PolyVector::const_iterator it=polygons.begin(), iend=polygons.end(); it!=iend; ++it)
	{
		Plane plane = make_plane(**it).to_undirected_form();
		planes.insert(plane);
	}

	return PlaneList_Ptr(new PlaneList(planes.begin(), planes.end()));
}

}
