/***
 * hesperus: CSGUtil.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#define CSGUtil_HEADER	template <typename Vert, typename AuxData>
#define CSGUtil_THIS	CSGUtil<Vert,AuxData>

#include <source/level/bsp/BSPBranch.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
CSGUtil_HEADER
typename CSGUtil_THIS::PolyList_Ptr
CSGUtil_THIS::union_all(const PolyBrushVector& brushes)
{
	PolyList_Ptr ret(new PolyList);

	// Build a tree for each brush.
	int brushCount = static_cast<int>(brushes.size());
	std::vector<BSPTree_Ptr> trees(brushCount);
	for(int i=0; i<brushCount; ++i)
	{
		trees[i] = build_tree(*brushes[i]);
	}

	// Determine which brushes are within range of each other.
	const double TOLERANCE = 1.0;
	std::vector<std::vector<unsigned char> > withinRange(brushCount);
	for(int i=0; i<brushCount; ++i)
	{
		withinRange[i].reserve(brushCount);
		for(int j=0; j<brushCount; ++j)
		{
			withinRange[i].push_back(AABB3d::within_range(brushes[i]->bounds(), brushes[j]->bounds(), TOLERANCE));
		}
	}

	// Clip each polygon to the tree of each brush within range of its own brush.
	for(int i=0; i<brushCount; ++i)
	{
		const PolyVector& faces = brushes[i]->faces();
		int faceCount = static_cast<int>(faces.size());
		for(int j=0; j<faceCount; ++j)
		{
			PolyList fragments;
			fragments.push_back(faces[j]);
			for(int k=0; k<brushCount; ++k)
			{
				if(k == i) continue;
				if(!withinRange[i][k]) continue;

				fragments = clip_to_tree(fragments, trees[k], i < k);
			}
			ret->splice(ret->end(), fragments);
		}
	}

	return ret;
}

//#################### PRIVATE METHODS ####################
CSGUtil_HEADER
BSPTree_Ptr CSGUtil_THIS::build_tree(const PolyBrush& brush)
{
	const PolyVector& faces = brush.faces();
	int faceCount = static_cast<int>(faces.size());

	int nodeCount = 2*faceCount+1;
	std::vector<BSPNode_Ptr> nodes(nodeCount);

	for(int i=0; i<faceCount; ++i) nodes[i] = BSPLeaf::make_empty_leaf(i, std::vector<int>());
	nodes[faceCount] = BSPLeaf::make_solid_leaf(faceCount);
	for(int i=faceCount+1; i<nodeCount; ++i)
	{
		Plane_Ptr splitter(new Plane(make_plane(*faces[nodeCount-i-1])));
		nodes[i].reset(new BSPBranch(i, splitter, nodes[nodeCount-i-1], nodes[i-1]));
	}

	return BSPTree_Ptr(new BSPTree(nodes));
}

CSGUtil_HEADER
typename CSGUtil_THIS::PolyList
CSGUtil_THIS::clip_to_subtree(const PolyList& polys, const BSPNode_Ptr& node, bool coplanarFlag)
{
	// NYI
	throw 23;
}

CSGUtil_HEADER
typename CSGUtil_THIS::PolyList
CSGUtil_THIS::clip_to_tree(const PolyList& polys, const BSPTree_Ptr& tree, bool coplanarFlag)
{
	return clip_to_subtree(polys, tree->root(), coplanarFlag);
}

}

#undef CSGUtil_THIS
#undef CSGUtil_HEADER
