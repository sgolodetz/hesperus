/***
 * hesperus: VisCalculator.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "VisCalculator.h"

#include <stack>

#include <source/math/geom/GeomUtil.h>
#include "Antipenumbra.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
VisCalculator::VisCalculator(const std::vector<Portal_Ptr>& portals)
:	m_portals(portals)
{}

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
void VisCalculator::calculate_portal_pvs(int originalSource)
{
	std::stack<PortalTriple> st;

	Portal_Ptr originalSourcePortal = m_portals[originalSource];
	Plane originalSourcePlane = make_plane(*originalSourcePortal);
	int origNeighbourLeaf = m_portals[originalSource]->auxiliary_data().toLeaf;
	const std::vector<int>& origCandidates = m_portalsFromLeaf[origNeighbourLeaf];
	for(size_t i=0, size=origCandidates.size(); i<size; ++i)
	{
		int target = origCandidates[i];
		if((*m_portalVis)(originalSource, target) != VS_NO)
		{
			Portal_Ptr targetPortal = m_portals[target];
			if((*m_classifiers)(originalSource, target) == CP_STRADDLE)
			{
				targetPortal = split_polygon(*targetPortal, originalSourcePlane).front;
			}
			st.push(PortalTriple(originalSourcePortal, Portal_Ptr(), targetPortal));
			(*m_portalVis)(originalSource, target) = VS_YES;
		}
	}

	while(!st.empty())
	{
		PortalTriple triple = st.top();
		Portal_Ptr source = triple.source, inter = triple.inter, target = triple.target;
		st.pop();

		Antipenumbra ap(source, target);

		// TODO
	}

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
	int portalCount = static_cast<int>(m_portals.size());
	for(int i=0; i<portalCount; ++i)
	{
		flood_from(i);

		// If any portals previously thought possible didn't get marked by the flood fill,
		// then they're not actually possible and need to be marked as such.
		for(int j=0; j<portalCount; ++j)
		{
			if((*m_portalVis)(i,j) == VS_INITIALMAYBE)
				(*m_portalVis)(i,j) = VS_NO;
		}
	}
}

/**
Peforms a flood fill from a given portal to refine its approximate
PVS before it is calculated for real.

@param originalSource	The portal from which to flood fill
*/
void VisCalculator::flood_from(int originalSource)
{
	std::stack<int> st;
	st.push(originalSource);

	while(!st.empty())
	{
		int curPortal = st.top();
		st.pop();

		if(curPortal != originalSource)
			(*m_portalVis)(originalSource, curPortal) = VS_FLOODFILLMAYBE;

		int leaf = m_portals[curPortal]->auxiliary_data().toLeaf;
		const std::vector<int>& candidates = m_portalsFromLeaf[leaf];
		for(size_t i=0, size=candidates.size(); i<size; ++i)
		{
			if((*m_portalVis)(originalSource, candidates[i]) == VS_INITIALMAYBE)
				st.push(candidates[i]);
		}
	}
}

/**
Performs the third (final) phase of the visibility calculation process,
in which the real PVS is calculated for each portal.
*/
void VisCalculator::full_portal_vis()
{
	int portalCount = static_cast<int>(m_portals.size());
	for(int i=0; i<portalCount; ++i)
	{
		calculate_portal_pvs(i);
	}
}

/**
Performs the first phase of the visibility calculation process. In this
phase, portals which obviously can't see each other (e.g. one portal
is fully behind another) are marked as such in the portal visibility
table. This helps avoid a lot of unnecessary clipping later on.
*/
void VisCalculator::initial_portal_vis()
{
	int portalCount = static_cast<int>(m_portals.size());
	m_portalVis.reset(new PortalVisTable(portalCount, VS_INITIALMAYBE));

	// Calculate the classification relation between the portals. Specifically,
	// classifiers(i,j) will contain the classification of polygon j relative
	// to the plane of i.

	// Note:	This bit could potentially be optimized if we required that portal pairs
	//			occupied consecutive indices in the list (e.g. if 1 were necessarily the
	//			reverse portal of 0, etc.).
	m_classifiers.reset(new ClassifierTable(portalCount));
	for(int i=0; i<portalCount; ++i)
	{
		const Plane plane = make_plane(*m_portals[i]);
		for(int j=0; j<portalCount; ++j)
		{
			if(j == i) (*m_classifiers)(i,j) = CP_COPLANAR;
			else (*m_classifiers)(i,j) = classify_polygon_against_plane(*m_portals[j], plane);
		}
	}

	// Run through the portal visibility table and mark (*m_portalVis)(i,j) as VS_NO
	// if portal i definitely can't see through portal j.
	for(int i=0; i<portalCount; ++i)
	{
		for(int j=0; j<portalCount; ++j)
		{
			if(j == i)
			{
				(*m_portalVis)(i,j) = VS_NO;
				continue;
			}

			// Note: Portals can only see through the back of other portals.

			// If portal j is behind or on the plane of portal i, then i can't see it.
			if((*m_classifiers)(i,j) == CP_BACK || (*m_classifiers)(i,j) == CP_COPLANAR)
				(*m_portalVis)(i,j) = VS_NO;

			// If portal i is completely in front of portal j, then it's facing i
			// and i can't see through it.
			if((*m_classifiers)(j,i) == CP_FRONT)
				(*m_portalVis)(i,j) = VS_NO;
		}
	}
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
