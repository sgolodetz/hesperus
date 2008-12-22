/***
 * hesperus: BSPNode.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "BSPNode.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BSPNode::BSPNode()
:	m_parent(NULL)
{}

//#################### PUBLIC METHODS ####################
BSPNode *BSPNode::parent()					{ return m_parent; }
const BSPNode *BSPNode::parent() const		{ return m_parent; }
void BSPNode::set_parent(BSPNode *parent)	{ m_parent = parent; }

}
