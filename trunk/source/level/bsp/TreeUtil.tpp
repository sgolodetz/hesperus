/***
 * hesperus: TreeUtil.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Finds the index of the leaf in the specified tree in which the specified point resides.

@param p		The point
@param tree		The tree
@return			The leaf index
*/
template <typename Tree>
int TreeUtil::find_leaf_index(const Vector3d& p, shared_ptr<Tree> tree)
{
	typedef typename Tree::Branch Branch;
	typedef typename Tree::Leaf Leaf;
	typedef typename Tree::Node Node;

	shared_ptr<Node> cur = tree->root();
	while(!cur->is_leaf())
	{
		const Branch *branch = cur->as_branch();
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

	const Leaf *leaf = cur->as_leaf();
	return leaf->leaf_index();
}

template <typename Tree>
std::list<Plane_CPtr> TreeUtil::split_planes(shared_ptr<Tree> tree)
{
	return split_planes_sub<Tree>(tree->root());
}

//#################### PRIVATE METHODS ####################
template <typename Tree>
std::list<Plane_CPtr> TreeUtil::split_planes_sub(shared_ptr<typename Tree::Node> node)
{
	std::list<Plane_CPtr> ret;

	if(!node->is_leaf())
	{
		const Tree::Branch *branch = node->as_branch();
		ret.push_back(branch->splitter());
		ret.splice(ret.end(), split_planes_sub<Tree>(branch->left()));
		ret.splice(ret.end(), split_planes_sub<Tree>(branch->right()));
	}

	return ret;
}

}
