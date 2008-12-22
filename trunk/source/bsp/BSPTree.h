/***
 * hesperus: BSPTree.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSP_BSPTREE
#define H_HESP_BSP_BSPTREE

#include "BSPNode.h"

namespace hesp {

class BSPTree
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<BSPNode_Ptr> m_nodes;

	//#################### CONSTRUCTORS ####################
public:
	BSPTree(const std::vector<BSPNode_Ptr>& nodes);

	//#################### PUBLIC METHODS ####################
public:
	void output_postorder_text(std::ostream& os) const;
	BSPNode_CPtr root() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<BSPTree> BSPTree_Ptr;
typedef shared_ptr<const BSPTree> BSPTree_CPtr;

}

#endif
