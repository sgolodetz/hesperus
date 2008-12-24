/***
 * hesperus: VisCalculator.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "VisCalculator.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Calculates the full leaf visibility table, which indicates which BSP leaves
can see each other in the game world.

@return	The leaf visibility table
*/
VisCalculator::LeafVisTable_Ptr VisCalculator::calculate_leaf_vis_table()
{
	if(!m_leafVis)
	{
		build_portals_from_leaf_lookup();
		initial_portal_vis();
		flood_fill();
		full_portal_vis();
		portal_to_leaf_vis();
		clean_intermediate();
	}

	return m_leafVis;
}

//#################### PRIVATE METHODS ####################
/**
Builds a table which allows us to look up which portals lead
out of a given leaf.
*/
void VisCalculator::build_portals_from_leaf_lookup()
{
	int portalCount = static_cast<int>(m_portals.size());
	for(int i=0; i<portalCount; ++i)
	{
		int fromLeaf = m_portals[i]->auxiliary_data().fromLeaf;
		m_portalsFromLeaf[fromLeaf].push_back(i);
	}
}

/**
Calculates the set of portals that are potentially visible from
the specified portal, and updates the portal visibility table
accordingly.

@param originalSource	The portal for which to calculate the PVS
*/
void VisCalculator::calculate_portal_pvs(const Portal_Ptr& originalSource)
{
	// NYI
	throw 23;
}

/**
Clears all the intermediate data structures used during the calculation process.
*/
void VisCalculator::clean_intermediate()
{
	// NYI
	throw 23;
}

/**
Performs the second phase of the visibility calculation process,
namely flood filling. This is used to refine the initial portal
visibility table before calculating the final version, the aim
being to speed up the final calculation process.
*/
void VisCalculator::flood_fill()
{
	// NYI
	throw 23;
}

/**
Peforms a flood fill from a given portal to refine its approximate
PVS before it is calculated for real.

@param originalSource	The portal from which to flood fill
*/
void VisCalculator::flood_from(const Portal_Ptr& originalSource)
{
	// NYI
	throw 23;
}

/**
Performs the third (final) phase of the visibility calculation process,
in which the real PVS is calculated for each portal.
*/
void VisCalculator::full_portal_vis()
{
	// NYI
	throw 23;
}

/**
Performs the first phase of the visibility calculation process. In this
phase, portals which obviously can't see each other (e.g. one portal
is fully behind another) are marked as such in the portal visibility
table. This helps avoid a lot of unnecessary clipping later on.
*/
void VisCalculator::initial_portal_vis()
{
	// NYI
	throw 23;
}

/**
Constructs a leaf visibility table from the portal one. The result of
the visibility calculation process is actually this leaf visibility
table, not the portal one, but the latter is more convenient during
the calculation process itself: we thus convert the one to the other
once we've finished calculating.
*/
void VisCalculator::portal_to_leaf_vis()
{
	// NYI
	throw 23;
}

}
