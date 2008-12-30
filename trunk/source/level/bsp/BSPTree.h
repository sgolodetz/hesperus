/***
 * hesperus: BSPTree.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_BSP_BSPTREE
#define H_HESP_LEVEL_BSP_BSPTREE

#include <source/math/vectors/Vector3.h>
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
	std::vector<BSPLeaf*> m_leaves;
	int m_emptyLeafCount;

	//#################### CONSTRUCTORS ####################
public:
	BSPTree(const std::vector<BSPNode_Ptr>& nodes);

	//#################### PUBLIC METHODS ####################
public:
	int empty_leaf_count() const;
	int find_leaf_index(const Vector3d& p) const;
	const BSPLeaf *leaf(int n) const;
	bool line_of_sight(const Vector3d& p1, const Vector3d& p2) const;
	static BSPTree_Ptr load_postorder_text(std::istream& is);
	void output_postorder_text(std::ostream& os) const;
	BSPNode_Ptr root() const;

	//#################### PRIVATE METHODS ####################
private:
	void index_leaves();
	void index_specific_leaves(const BSPNode_Ptr& node, bool solidFlag);
};

}

#endif
