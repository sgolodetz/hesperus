/***
 * hesperus: BSPUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BSPUtil.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Finds the index of the leaf in the specified BSP tree in which the specified point resides.

@param p		The point
@param tree		The BSP tree
@return			The leaf index
*/
int BSPUtil::find_leaf_index(const Vector3d& p, const BSPTree_CPtr& tree)
{
	BSPNode_Ptr cur = tree->root();
	while(!cur->is_leaf())
	{
		const BSPBranch *branch = cur->as_branch();
		switch(classify_point_against_plane(p, *branch->splitter()))
		{
			case CP_BACK:
			{
				cur = branch->right();
				break;
			}
			default:	// CP_COPLANAR or CP_FRONT
			{
				cur = branch->left();
				break;
			}
		}
	}

	const BSPLeaf *leaf = cur->as_leaf();
	return leaf->leaf_index();
}

/**
Determines whether or not there is line-of-sight between p1 and p2 in the specified BSP tree.

@param p1		A point in the world
@param p2		Another point in the world
@param tree		The BSP tree
@return			true, if there is line-of-sight between the points, or false otherwise
*/
bool BSPUtil::line_of_sight(const Vector3d& p1, const Vector3d& p2, const BSPTree_CPtr& tree)
{
	return line_of_sight_sub(p1, p2, tree->root());
}

//#################### PRIVATE METHODS ####################
bool BSPUtil::line_of_sight_sub(const Vector3d& p1, const Vector3d& p2, const BSPNode_Ptr& node)
{
	if(node->is_leaf())
	{
		// If both points are in the same leaf, there's line-of-sight provided the leaf's not solid.
		const BSPLeaf *leaf = node->as_leaf();
		return !leaf->is_solid();
	}

	const BSPBranch *branch = node->as_branch();
	PlaneClassifier cp1, cp2;
	switch(classify_linesegment_against_plane(p1, p2, *branch->splitter(), cp1, cp2))
	{
		case CP_BACK:
		{
			return line_of_sight_sub(p1, p2, branch->right());
		}
		case CP_COPLANAR:
		case CP_FRONT:
		{
			return line_of_sight_sub(p1, p2, branch->left());
		}
		default:	// case CP_STRADDLE
		{
			Vector3d q = determine_linesegment_intersection_with_plane(p1, p2, *branch->splitter()).first;
			if(cp1 == CP_BACK)
			{
				// cp2 == CP_FRONT
				return line_of_sight_sub(p1, q, branch->right()) && line_of_sight_sub(q, p2, branch->left());
			}
			else
			{
				// cp1 == CP_FRONT, cp2 == CP_BACK
				return line_of_sight_sub(p1, q, branch->left()) && line_of_sight_sub(q, p2, branch->right());
			}
		}
	}
}

}
