/***
 * hesperus: BSPCompiler.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_BSP_BSPCOMPILER
#define H_HESP_LEVEL_BSP_BSPCOMPILER

#include <source/math/geom/Polygon.h>
#include "BSPTree.h"

namespace hesp {

class BSPCompiler
{
	//#################### NESTED CLASSES ####################
private:
	struct PolyIndex
	{
		int index;
		bool splitCandidate;	// is the plane of the referenced polygon a split candidate?

		PolyIndex(int index_, bool splitCandidate_)
		:	index(index_), splitCandidate(splitCandidate_)
		{}
	};

	//#################### PUBLIC METHODS ####################
public:
	template <typename Vert, typename AuxData>
	static BSPTree_Ptr build_tree(std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, const double weight);

	//#################### PRIVATE METHODS ####################
private:
	template <typename Vert, typename AuxData>
	static BSPNode_Ptr build_subtree(const std::vector<PolyIndex>& polyIndices, std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, const double weight,
									 std::vector<BSPNode_Ptr>& nodes, PlaneClassifier relativeToParent = CP_BACK);

	template <typename Vert, typename AuxData>
	static Plane_Ptr choose_split_plane(const std::vector<PolyIndex>& polyIndices, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, const double weight);
};

}

#include "BSPCompiler.tpp"

#endif
