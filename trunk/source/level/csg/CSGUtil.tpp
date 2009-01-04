/***
 * hesperus: CSGUtil.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#define CSGUtil_HEADER	template <typename Vert, typename AuxData>
#define CSGUtil_THIS	CSGUtil<Vert,AuxData>

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
	std::vector<std::vector<unsigned char> > withinRange(brushCount);
	for(int i=0; i<brushCount; ++i)
	{
		withinRange[i].reserve(brushCount);
		for(int j=0; j<brushCount; ++j)
		{
			withinRange.push_back(AABB::within_range(brushes[i]->bounds(), brushes[j]->bounds()));
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
			ret.splice(ret.end(), fragments);
		}
	}

	return ret;
}

//#################### PRIVATE METHODS ####################
CSGUtil_HEADER
BSPTree_Ptr CSGUtil_THIS::build_tree(const PolyBrush& brush)
{
	// NYI
	throw 23;
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
