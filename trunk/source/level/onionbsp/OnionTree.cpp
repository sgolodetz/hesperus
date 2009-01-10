/***
 * hesperus: OnionTree.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "OnionTree.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
OnionTree::OnionTree(const std::vector<OnionNode_Ptr>& nodes, int mapCount)
:	m_nodes(nodes), m_mapCount(mapCount)
{
	index_leaves();
}

//#################### PUBLIC METHODS ####################
void OnionTree::output_postorder_text(std::ostream& os) const
{
	os << m_mapCount << '\n';
	os << m_nodes.size() << '\n';
	root()->output_postorder_text(os);
}

OnionNode_Ptr OnionTree::root() const
{
	return m_nodes.back();
}

//#################### PRIVATE METHODS ####################
void OnionTree::index_leaves()
{
	index_leaves_sub(root());
}

void OnionTree::index_leaves_sub(const OnionNode_Ptr& node)
{
	if(node->is_leaf())
	{
		OnionLeaf *leaf = node->as_leaf();
		int nextLeaf = static_cast<int>(m_leaves.size());
		leaf->set_leaf_index(nextLeaf);
		m_leaves.push_back(leaf);
	}
	else
	{
		const OnionBranch *branch = node->as_branch();
		index_leaves_sub(branch->left());
		index_leaves_sub(branch->right());
	}
}

}
