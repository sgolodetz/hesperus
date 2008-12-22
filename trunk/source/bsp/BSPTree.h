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
	BSPNode_Ptr m_root;

	//#################### CONSTRUCTORS ####################
public:
	BSPTree(const BSPNode_Ptr& root);

	//#################### PUBLIC METHODS ####################
public:
	void output_postorder_text(std::ostream& os) const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<BSPTree> BSPTree_Ptr;
typedef shared_ptr<const BSPTree> BSPTree_CPtr;

}

#endif
