/***
 * hesperus: OnionCompiler.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

namespace hesp {

//#################### CONSTRUCTORS ####################
template <typename Poly>
OnionCompiler<Poly>::OnionCompiler(const std::vector<PolyVector_Ptr>& maps, double weight)
:	m_weight(weight), m_mapCount(static_cast<int>(maps.size())), m_polygons(new PolyVector)
{
	std::set<OnionPlane> onionPlanes;

	int mapCount = static_cast<int>(maps.size());
	for(int i=0; i<mapCount; ++i)
	{
		for(PolyVector::const_iterator jt=maps[i]->begin(), jend=maps[i]->end(); ++jt)
		{
			const Poly_Ptr& poly = *jt;
			m_polygons->push_back(poly);

			// TODO:	Determine the polygon's onion plane.

			// TODO:	Try adding it to the onion plane set. If it's not present,
			//			add the new onion plane to the onion plane vector. If it
			//			is, add the index of this map to the existing onion plane
			//			and use that.

			// TODO:	Fill in the polygon -> onion plane map accordingly.
		}
	}

	// NYI
	throw 23;
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
	// NYI
	throw 23;
}

template <typename Poly>
OnionPlane_Ptr OnionCompiler<Poly>::choose_split_plane(const std::vector<PolyIndex>& polyIndices)
{
	// NYI
	throw 23;
}

}
