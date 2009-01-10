/***
 * hesperus: BasePortalGenerator.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#define BPG_HEADER	template <typename PortalT, typename TreeT, typename NodeT>
#define BPG_THIS	BasePortalGenerator<PortalT, TreeT, NodeT>

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Generates a list of portals for a 3D world.

@param polygons	The world polygons
@param tree		The tree for the world
@return			As stated
*/
BPG_HEADER
template <typename Poly>
typename BPG_THIS::PortalTList_Ptr
BPG_THIS::generate_portals(const std::vector<shared_ptr<Poly> >& polygons, const TreeT_Ptr& tree) const
{
	PortalTList_Ptr portals(new PortalTList);
	PlaneList_Ptr planes = find_unique_planes(polygons);

	for(PlaneList::const_iterator it=planes->begin(), iend=planes->end(); it!=iend; ++it)
	{
		PortalT_Ptr portal = make_initial_portal(*it);
		portals->splice(portals->end(), clip_portal_to_tree(portal, tree));
	}

	// Generate the opposite-facing portals.
	for(PortalTList::iterator it=portals->begin(), iend=portals->end(); it!=iend; ++it)
	{
		PortalT_Ptr portal = *it;

		// Construct the reverse portal.
		PortalT_Ptr reversePortal(portal->flipped_winding());
		reversePortal->auxiliary_data() = portal->auxiliary_data().flip();

		// Insert it after the existing portal in the list.
		++it;
		it = portals->insert(it, reversePortal);
	}

	return portals;
}

//#################### PRIVATE METHODS ####################
/**
Clips the portal to the tree and returns a list of portal fragments which survive the clipping process.

@param portal	The portal to clip
@param tree		The tree to which to clip it
@return			The list of portal fragments which survived the clipping process
*/
BPG_HEADER
typename BPG_THIS::PortalTList
BPG_THIS::clip_portal_to_tree(const PortalT_Ptr& portal, const TreeT_Ptr& tree) const
{
	return clip_portal_to_subtree(portal, tree->root());
}

/**
Makes an initial portal on a given plane. This portal should be large enough to
span the entire level space.

@param plane	The plane on which to build the initial portal
@return			As stated
*/
BPG_HEADER
typename BPG_THIS::PortalT_Ptr
BPG_THIS::make_initial_portal(const Plane& plane) const
{
	return make_universe_polygon<typename PortalT::AuxData>(plane);
}

}

#undef BPG_THIS
#undef BPG_HEADER
