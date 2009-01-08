/***
 * hesperus: OnionCompiler.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
template <typename Poly>
OnionCompiler<Poly>::OnionCompiler(const std::vector<PolyVector>& maps, double weight)
:	m_weight(weight), m_mapCount(static_cast<int>(maps.size())), m_polygons(new PolyVector)
{
	typedef std::map<OnionPlane_Ptr,int,OnionPlanePred> OnionPlaneMap;

	// Build the set of unique onion planes, and a map from polygons to their respective planes.
	OnionPlaneMap onionPlanes;

	int mapCount = static_cast<int>(maps.size());
	for(int i=0; i<mapCount; ++i)
	{
		for(PolyVector::const_iterator jt=maps[i].begin(), jend=maps[i].end(); jt!=jend; ++jt)
		{
			const Poly_Ptr& poly = *jt;
			int polyIndex = static_cast<int>(m_polygons->size());
			m_polygons->push_back(poly);

			// Determine the polygon's (undirected) onion plane.
			OnionPlane_Ptr onionPlane(new OnionPlane(make_plane(*poly).to_undirected_form(), i));

			// Try and add the onion plane to the set of unique onion planes.
			int onionPlaneIndex = static_cast<int>(onionPlanes.size());
			std::pair<OnionPlaneMap::iterator,bool> kt = onionPlanes.insert(std::make_pair(onionPlane,onionPlaneIndex));

			// If this onion plane duplicates an existing one, simply add this map's index to that plane.
			if(kt.second == false)
			{
				kt.first->first->add_map_index(i);
			}

			// In either case, this polygon's onion plane index is the one returned when we tried to do the insert.
			m_polyToOnionPlaneIndex.insert(std::make_pair(polyIndex, kt.first->second));
		}
	}

	m_onionPlanes.resize(onionPlanes.size());
	for(OnionPlaneMap::const_iterator it=onionPlanes.begin(), iend=onionPlanes.end(); it!=iend; ++it)
	{
		m_onionPlanes[it->second] = it->first;
	}
}

//#################### PUBLIC METHODS ####################
template <typename Poly>
void OnionCompiler<Poly>::build_tree()
{
	int polyCount = static_cast<int>(m_polygons->size());
	std::vector<PolyIndex> polyIndices;
	polyIndices.reserve(polyCount);
	for(int i=0; i<polyCount; ++i) polyIndices.push_back(PolyIndex(i, true));

	std::vector<OnionNode_Ptr> nodes;
	std::vector<PlaneClassifier> relativeToClosestAncestor(m_mapCount, CP_BACK);

	build_subtree(polyIndices, nodes, relativeToClosestAncestor);

	m_tree.reset(new OnionTree(nodes));
}

template <typename Poly>
const typename OnionCompiler<Poly>::PolyVector_Ptr&
OnionCompiler<Poly>::polygons() const
{
	return m_polygons;
}

template <typename Poly>
const OnionTree_Ptr& OnionCompiler<Poly>::tree() const
{
	return m_tree;
}

//#################### PRIVATE METHODS ####################
template <typename Poly>
OnionNode_Ptr
OnionCompiler<Poly>::build_subtree(const std::vector<PolyIndex>& polyIndices, std::vector<OnionNode_Ptr>& nodes,
								   const std::vector<PlaneClassifier>& relativeToClosestAncestor)
{
	OnionPlane_Ptr splitter = choose_split_plane(polyIndices);

	// NYI
	throw 23;
}

template <typename Poly>
OnionPlane_Ptr OnionCompiler<Poly>::choose_split_plane(const std::vector<PolyIndex>& polyIndices)
{
	OnionPlane_Ptr bestOnionPlane;
	double bestMetric = INT_MAX;

	int indexCount = static_cast<int>(polyIndices.size());
	for(int i=0; i<indexCount; ++i)
	{
		if(!polyIndices[i].splitCandidate) continue;

		OnionPlane_Ptr onionPlane = m_onionPlanes[m_polyToOnionPlaneIndex[polyIndices[i].index]];
		int balance = 0, splits = 0;

		for(int j=0; j<indexCount; ++j)
		{
			if(j == i) continue;

			Poly_Ptr poly = (*m_polygons)[polyIndices[j].index];
			switch(classify_polygon_against_plane(*poly, onionPlane->plane()))
			{
				case CP_BACK:
					--balance;
					break;
				case CP_COPLANAR:
					break;
				case CP_FRONT:
					++balance;
					break;
				case CP_STRADDLE:
					++splits;
					break;
			}
		}

		double metric = abs(balance) + m_weight * splits;
		if(metric < bestMetric)
		{
			bestOnionPlane = onionPlane;
			bestMetric = metric;
		}
	}

	return bestOnionPlane;
}

}
