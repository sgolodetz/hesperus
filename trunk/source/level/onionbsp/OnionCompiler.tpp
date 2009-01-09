/***
 * hesperus: OnionCompiler.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <source/math/geom/GeomUtil.h>
#include "OnionBranch.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
template <typename Poly>
OnionCompiler<Poly>::OnionCompiler(const std::vector<PolyVector>& maps, double weight)
:	m_weight(weight), m_mapCount(static_cast<int>(maps.size())), m_polygons(new PolyVector)
{
	int mapCount = static_cast<int>(maps.size());
	for(int i=0; i<mapCount; ++i)
	{
		for(PolyVector::const_iterator jt=maps[i].begin(), jend=maps[i].end(); jt!=jend; ++jt)
		{
			const Poly_Ptr& poly = *jt;
			int polyIndex = static_cast<int>(m_polygons->size());
			m_polygons->push_back(poly);
			m_polyIndices.push_back(PolyIndex(polyIndex, i, true));
		}
	}
}

//#################### PUBLIC METHODS ####################
template <typename Poly>
void OnionCompiler<Poly>::build_tree()
{
	std::vector<OnionNode_Ptr> nodes;

	boost::dynamic_bitset<> solidityDescriptor(m_mapCount);
	solidityDescriptor.set();

	build_subtree(m_polyIndices, nodes, solidityDescriptor);

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
								   const boost::dynamic_bitset<>& solidityDescriptor)
{
	typedef typename Poly::Vert Vert;
	typedef typename Poly::AuxData AuxData;

#ifdef ONION_TRACE
	std::cout << solidityDescriptor.to_ulong();
#endif

	OnionPlane_Ptr splitter = choose_split_plane(polyIndices);

	if(!splitter)
	{
#ifdef ONION_TRACE
		std::cout << '\n';
#endif

		std::vector<int> indicesOnly;
		for(size_t i=0, size=polyIndices.size(); i<size; ++i) indicesOnly.push_back(polyIndices[i].index);
		nodes.push_back(OnionNode_Ptr(new OnionLeaf((int)nodes.size(), solidityDescriptor, indicesOnly)));
		return nodes.back();
	}

#ifdef ONION_TRACE
	std::cout << ' ' << splitter->plane() << ' ' << splitter->map_index() << '\n';
#endif

	std::vector<PolyIndex> backPolys, frontPolys;

	for(std::vector<PolyIndex>::const_iterator it=polyIndices.begin(), iend=polyIndices.end(); it!=iend; ++it)
	{
		int curIndex = it->index;
		int curMapIndex = it->mapIndex;
		const Poly& curPoly = *(*m_polygons)[curIndex];
		switch(classify_polygon_against_plane(curPoly, splitter->plane()))
		{
			case CP_BACK:
			{
				backPolys.push_back(*it);
				break;
			}
			case CP_COPLANAR:
			{
				// If this polygon was a split candidate and is in a different map from the
				// splitter, then it's still a split candidate afterwards.
				bool futureSplitCandidate = it->splitCandidate && curMapIndex != splitter->map_index();
				if(splitter->plane().normal().dot(curPoly.normal()) > 0)
					frontPolys.push_back(PolyIndex(curIndex,curMapIndex,futureSplitCandidate));
				else
					backPolys.push_back(PolyIndex(curIndex,curMapIndex,futureSplitCandidate));
				break;
			}
			case CP_FRONT:
			{
				frontPolys.push_back(*it);
				break;
			}
			case CP_STRADDLE:
			{
				SplitResults<Vert,AuxData> sr = split_polygon(curPoly, splitter->plane());

				// Copy the back half over the polygon being split.
				(*m_polygons)[curIndex] = sr.back;

				// Append the front half to the end of the polygon array.
				int k = static_cast<int>(m_polygons->size());
				m_polygons->push_back(sr.front);

				backPolys.push_back(PolyIndex(curIndex, curMapIndex, it->splitCandidate));
				frontPolys.push_back(PolyIndex(k, curMapIndex, it->splitCandidate));
				break;
			}
		}
	}

	boost::dynamic_bitset<> leftSolidityDescriptor = solidityDescriptor;
	boost::dynamic_bitset<> rightSolidityDescriptor = solidityDescriptor;
	int mapIndex = splitter->map_index();
	leftSolidityDescriptor.set(mapIndex, false);
	rightSolidityDescriptor.set(mapIndex, true);

	OnionNode_Ptr left = build_subtree(frontPolys, nodes, leftSolidityDescriptor);
	OnionNode_Ptr right = build_subtree(backPolys, nodes, rightSolidityDescriptor);

	OnionNode_Ptr subtreeRoot(new OnionBranch((int)nodes.size(), Plane_Ptr(new Plane(splitter->plane())), left, right));
	nodes.push_back(subtreeRoot);
	return subtreeRoot;
}

template <typename Poly>
OnionPlane_Ptr OnionCompiler<Poly>::choose_split_plane(const std::vector<PolyIndex>& polyIndices)
{
	OnionPlane_Ptr bestOnionPlane;
	int bestMapIndex = INT_MAX;
	double bestMetric = INT_MAX;

	int indexCount = static_cast<int>(polyIndices.size());
	for(int i=0; i<indexCount; ++i)
	{
		if(!polyIndices[i].splitCandidate) continue;

		OnionPlane_Ptr onionPlane(new OnionPlane(make_plane(*(*m_polygons)[polyIndices[i].index]), polyIndices[i].mapIndex));

		int balance = 0, splits = 0;
		for(int j=0; j<indexCount; ++j)
		{
			if(j == i) continue;

			switch(classify_polygon_against_plane(*(*m_polygons)[polyIndices[j].index], onionPlane->plane()))
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

		// We want to process all the planes that appear in each map in order before moving on to
		// the next one: this is essential in order to ensure a correct tree. We therefore give
		// priority to onion planes with a lower map index (i.e. we order planes first by lowest
		// map index, and only then by metric).
		int mapIndex = onionPlane->map_index();
		double metric = abs(balance) + m_weight * splits;

		if(mapIndex < bestMapIndex || (mapIndex == bestMapIndex && metric < bestMetric))
		{
			bestOnionPlane = onionPlane;
			bestMapIndex = mapIndex;
			bestMetric = metric;
		}
	}

	return bestOnionPlane;
}

}
