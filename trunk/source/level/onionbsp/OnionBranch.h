/***
 * hesperus: OnionBranch.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_ONIONBSP_ONIONBRANCH
#define H_HESP_LEVEL_ONIONBSP_ONIONBRANCH

#include <source/math/geom/Plane.h>
#include "OnionNode.h"

namespace hesp {

class OnionBranch : public OnionNode
{
	//#################### PRIVATE VARIABLES ####################
private:
	OnionNode_Ptr m_left;
	OnionNode_Ptr m_right;
	Plane_Ptr m_splitter;

	//#################### CONSTRUCTORS ####################
public:
	OnionBranch(int index, const Plane_Ptr& splitter, const OnionNode_Ptr& left, const OnionNode_Ptr& right);

	//#################### PUBLIC METHODS ####################
public:
	OnionBranch *as_branch();
	const OnionBranch *as_branch() const;
	OnionLeaf *as_leaf();
	const OnionLeaf *as_leaf() const;
	bool is_leaf() const;
	const OnionNode_Ptr& left() const;
	void output_postorder_text(std::ostream& os) const;
	const OnionNode_Ptr& right() const;
	Plane_CPtr splitter() const;
};

}

#endif
