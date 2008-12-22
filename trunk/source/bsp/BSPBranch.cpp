/***
 * hesperus: BSPBranch.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "BSPBranch.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BSPBranch::BSPBranch(const BSPNode_Ptr& left, const BSPNode_Ptr& right)
:	m_left(left), m_right(right)
{}

//#################### PUBLIC METHODS ####################
BSPBranch *BSPBranch::as_branch()				{ return this; }
const BSPBranch *BSPBranch::as_branch() const	{ return this; }
BSPLeaf *BSPBranch::as_leaf()					{ return NULL; }
const BSPLeaf *BSPBranch::as_leaf() const		{ return NULL; }
bool BSPBranch::is_leaf() const					{ return false; }
BSPNode *BSPBranch::left()						{ return m_left.get(); }
const BSPNode *BSPBranch::left() const			{ return m_left.get(); }
BSPNode *BSPBranch::right()						{ return m_right.get(); }
const BSPNode *BSPBranch::right() const			{ return m_right.get(); }

}
