/***
 * hesperus: BSPBranch.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSP_BSPBRANCH
#define H_HESP_BSP_BSPBRANCH

#include "BSPNode.h"

namespace hesp {

class BSPBranch : public BSPNode
{
	//#################### PRIVATE VARIABLES ####################
private:
	BSPNode_Ptr m_left;
	BSPNode_Ptr m_right;

	//#################### CONSTRUCTORS ####################
public:
	BSPBranch(const BSPNode_Ptr& left, const BSPNode_Ptr& right);

	//#################### PUBLIC METHODS ####################
public:
	BSPBranch *as_branch();
	const BSPBranch *as_branch() const;
	BSPLeaf *as_leaf();
	const BSPLeaf *as_leaf() const;
	bool is_leaf() const;
	BSPNode *left();
	const BSPNode *left() const;
	BSPNode *right();
	const BSPNode *right() const;
};

}

#endif
