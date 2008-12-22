/***
 * hesperus: BSPTree.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "BSPTree.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BSPTree::BSPTree(const BSPNode_Ptr& root)
:	m_root(root)
{}

//#################### PUBLIC METHODS ####################
void BSPTree::output_postorder_text(std::ostream& os) const
{
	// TODO
}

}
