/***
 * hesperus: BSPLeaf.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSP_BSPLEAF
#define H_HESP_BSP_BSPLEAF

#include "BSPNode.h"

namespace hesp {

class BSPLeaf : public BSPNode
{
	//#################### PRIVATE VARIABLES ####################
private:
	bool m_isSolid;
	std::vector<int> m_polygonIndices;

	//#################### CONSTRUCTORS ####################
private:
	BSPLeaf() {}

	//#################### STATIC FACTORY METHODS ####################
public:
	static BSPNode_Ptr make_empty_leaf(const std::vector<int>& polygonIndices);
	static BSPNode_Ptr make_solid_leaf();

	//#################### PUBLIC METHODS ####################
public:
	BSPBranch *as_branch();
	const BSPBranch *as_branch() const;
	BSPLeaf *as_leaf();
	const BSPLeaf *as_leaf() const;
	bool is_leaf() const;
	bool is_solid() const;
	void map_polygon_indices(const std::map<int,int>& indexMap);
	const std::vector<int>& polygon_indices() const;
};

}

#endif
