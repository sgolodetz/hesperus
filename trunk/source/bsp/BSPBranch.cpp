/***
 * hesperus: BSPBranch.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "BSPBranch.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BSPBranch::BSPBranch(int index, const Plane_Ptr& splitter, const BSPNode_Ptr& left, const BSPNode_Ptr& right)
:	BSPNode(index), m_splitter(splitter), m_left(left), m_right(right)
{}

//#################### PUBLIC METHODS ####################
const BSPBranch *BSPBranch::as_branch() const	{ return this; }
const BSPLeaf *BSPBranch::as_leaf() const		{ return NULL; }
bool BSPBranch::is_leaf() const					{ return false; }
const BSPNode *BSPBranch::left() const			{ return m_left.get(); }

void BSPBranch::output_postorder_text(std::ostream& os) const
{
	if(m_left.get()) m_left->output_postorder_text(os);
	if(m_right.get()) m_right->output_postorder_text(os);

	int parentIndex = m_parent != NULL ? m_parent->index() : -1;
	os << m_index << " B " << m_left->index() << ' ' << m_right->index() << ' ' << parentIndex;

	const Vector3d& n = m_splitter->normal();
	double d = m_splitter->distance_value();
	os << " ( " << n.x << ' ' << n.y << ' ' << n.z << ' ' << d << " )";

	os << '\n';
}

const BSPNode *BSPBranch::right() const			{ return m_right.get(); }
Plane_CPtr BSPBranch::splitter() const			{ return m_splitter; }

}
