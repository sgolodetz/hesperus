/***
 * hesperus: BSPTree.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "BSPTree.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BSPTree::BSPTree(const std::vector<BSPNode_Ptr>& nodes)
:	m_nodes(nodes)
{}

//#################### PUBLIC METHODS ####################
void BSPTree::output_postorder_text(std::ostream& os) const
{
	root()->output_postorder_text(os);
}

BSPNode_CPtr BSPTree::root() const
{
	return m_nodes.back();
}

}
