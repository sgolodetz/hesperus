/***
 * hesperus: PortalGenerator.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <set>

#include <source/math/geom/GeomUtil.h>
#include <source/math/geom/UniquePlanePred.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Generates a list of portals for a 3D world.

@param polygons	The world polygons
@param tree		The BSP tree for the world
@return			As stated
*/
template <typename Poly>
typename PortalGenerator::PortalList_Ptr
PortalGenerator::generate_portals(const std::vector<shared_ptr<Poly> >& polygons, const BSPTree_Ptr& tree)
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
		const PortalInfo& portalInfo = portal->auxiliary_data();
		reversePortal->auxiliary_data() = PortalInfo(portalInfo.toLeaf, portalInfo.fromLeaf);

		// Insert it after the existing portal in the list.
		++it;
		it = portals->insert(it, reversePortal);
	}

	return portals;
}

}
