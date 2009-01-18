/***
 * hesperus: OnionTree.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_ONIONBSP_ONIONTREE
#define H_HESP_LEVEL_ONIONBSP_ONIONTREE

#include <source/math/vectors/Vector3.h>
#include "OnionBranch.h"
#include "OnionLeaf.h"

namespace hesp {

//#################### TYPEDEFS ####################
typedef shared_ptr<class OnionTree> OnionTree_Ptr;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;

class OnionTree
{
	//#################### TYPEDEFS ####################
public:
	typedef OnionBranch Branch;
	typedef OnionLeaf Leaf;
	typedef OnionNode Node;

	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<OnionNode_Ptr> m_nodes;
	std::vector<OnionLeaf*> m_leaves;
	int m_mapCount;

	//#################### CONSTRUCTORS ####################
public:
	OnionTree(const std::vector<OnionNode_Ptr>& nodes, int mapCount);

	//#################### PUBLIC METHODS ####################
public:
	int find_leaf_index(const Vector3d& p) const;
	const OnionLeaf *leaf(int n) const;
	bool line_of_sight(int mapIndex, const Vector3d& p1, const Vector3d& p2) const;
	static OnionTree_Ptr load_postorder_text(std::istream& is);
	int map_count() const;
	void output_postorder_text(std::ostream& os) const;
	OnionNode_Ptr root() const;

	//#################### PRIVATE METHODS ####################
private:
	void index_leaves();
	void index_leaves_sub(const OnionNode_Ptr& node);
	bool line_of_sight_sub(int mapIndex, const Vector3d& p1, const Vector3d& p2, const OnionNode_Ptr& node) const;
};

}

#endif
