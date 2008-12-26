/***
 * hesperus: BSPTree.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSP_BSPTREE
#define H_HESP_BSP_BSPTREE

#include "BSPLeaf.h"

namespace hesp {

//#################### TYPEDEFS ####################
typedef shared_ptr<class BSPTree> BSPTree_Ptr;
typedef shared_ptr<const class BSPTree> BSPTree_CPtr;

class BSPTree
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<BSPNode_Ptr> m_nodes;
	std::vector<BSPLeaf*> m_emptyLeaves;

	//#################### CONSTRUCTORS ####################
public:
	BSPTree(const std::vector<BSPNode_Ptr>& nodes);

	//#################### PUBLIC METHODS ####################
public:
	static BSPTree_Ptr load_postorder_text(std::istream& is);
	void output_postorder_text(std::ostream& os) const;
	BSPNode_Ptr root() const;

	//#################### PRIVATE METHODS ####################
private:
	void index_empty_leaves(const BSPNode_Ptr& node);
};

}

#endif
