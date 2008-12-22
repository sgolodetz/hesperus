/***
 * hesperus: BSPCompiler.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "BSPBranch.h"
#include "BSPLeaf.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
template <typename Vert, typename AuxData>
BSPTree_Ptr BSPCompiler::build_tree(std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, const double weight)
{
	int polyCount = static_cast<int>(polygons.size());
	std::vector<PolyIndex> polyIndices;
	polyIndices.reserve(polyCount);
	for(int i=0; i<polyCount; ++i) polyIndices.push_back(PolyIndex(i, true));
	std::vector<BSPNode_Ptr> nodes;
	build_subtree(polyIndices, polygons, weight, nodes);
	return BSPTree_Ptr(new BSPTree(nodes));
}

//#################### PRIVATE METHODS ####################
template <typename Vert, typename AuxData>
BSPNode_Ptr BSPCompiler::build_subtree(const std::vector<PolyIndex>& polyIndices, std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, const double weight,
									   std::vector<BSPNode_Ptr>& nodes, PlaneClassifier relativeToParent)
{
	Plane_Ptr splitter = choose_split_plane(polyIndices, polygons, weight);

	if(splitter.get() == NULL)
	{
		if(relativeToParent == CP_BACK)
		{
			nodes.push_back(BSPLeaf::make_solid_leaf((int)nodes.size()));
		}
		else
		{
			std::vector<int> indicesOnly;
			for(size_t i=0, size=polyIndices.size(); i<size; ++i) indicesOnly.push_back(polyIndices[i].index);
			nodes.push_back(BSPLeaf::make_empty_leaf((int)nodes.size(), indicesOnly));
		}
		return nodes.back();
	}

	std::vector<PolyIndex> backPolys, frontPolys;

	for(std::vector<PolyIndex>::const_iterator it=polyIndices.begin(), iend=polyIndices.end(); it!=iend; ++it)
	{
		int curIndex = it->index;
		const Polygon<Vert,AuxData>& curPoly = *polygons[curIndex];
		switch(classify_polygon_against_plane(curPoly, *splitter))
		{
			case CP_BACK:
			{
				backPolys.push_back(*it);
				break;
			}
			case CP_COPLANAR:
			{
				if(splitter->normal().dot(curPoly.normal()) > 0) frontPolys.push_back(PolyIndex(curIndex,false));
				else backPolys.push_back(PolyIndex(curIndex,false));
				break;
			}
			case CP_FRONT:
			{
				frontPolys.push_back(*it);
				break;
			}
			case CP_STRADDLE:
			{
				SplitResults<Vert,AuxData> sr = split_polygon(curPoly, *splitter);
				polygons[curIndex] = sr.back;
				int k = static_cast<int>(polygons.size());
				polygons.push_back(sr.front);
				backPolys.push_back(PolyIndex(curIndex,it->splitCandidate));
				frontPolys.push_back(PolyIndex(k,it->splitCandidate));
				break;
			}
		}
	}

	BSPNode_Ptr left = build_subtree(frontPolys, polygons, weight, nodes, CP_FRONT);
	BSPNode_Ptr right = build_subtree(backPolys, polygons, weight, nodes, CP_BACK);

	BSPNode_Ptr subtreeRoot(new BSPBranch((int)nodes.size(), splitter, left, right));
	left->set_parent(subtreeRoot.get());
	right->set_parent(subtreeRoot.get());
	nodes.push_back(subtreeRoot);
	return subtreeRoot;
}

template <typename Vert, typename AuxData>
Plane_Ptr BSPCompiler::choose_split_plane(const std::vector<PolyIndex>& polyIndices, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, const double weight)
{
	Plane_Ptr bestPlane;
	double bestMetric = INT_MAX;

	int indexCount = static_cast<int>(polyIndices.size());
	for(int i=0; i<indexCount; ++i)
	{
		if(!polyIndices[i].splitCandidate) continue;

		Plane plane = make_plane(*polygons[polyIndices[i].index]);
		int balance = 0, splits = 0;

		for(int j=0; j<indexCount; ++j)
		{
			if(j == i) continue;

			switch(classify_polygon_against_plane(*polygons[polyIndices[j].index], plane))
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

		double metric = weight*balance + splits;
		if(metric < bestMetric)
		{
			bestPlane = Plane_Ptr(new Plane(plane));
			bestMetric = metric;
		}
	}

	return bestPlane;
}

}
