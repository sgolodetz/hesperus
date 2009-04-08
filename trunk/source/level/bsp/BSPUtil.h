/***
 * hesperus: BSPUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BSPUTIL
#define H_HESP_BSPUTIL

#include <list>

namespace hesp {

class BSPUtil
{
	//#################### PUBLIC METHODS ####################
public:
	template <typename Vert, typename AuxData> static std::list<int> find_leaf_indices(const Polygon<Vert,AuxData>& poly, const BSPTree_Ptr& tree);

	//#################### PRIVATE METHODS ####################
private:
	template <typename Vert, typename AuxData> static std::list<int> find_leaf_indices(const Polygon<Vert,AuxData>& poly, const BSPNode_Ptr& node);
};

}

#include "BSPUtil.tpp"

#endif
