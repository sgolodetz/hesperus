/***
 * hesperus: BSPTree.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "BSPTree.h"

#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>
#include <source/math/geom/GeomUtil.h>
#include "BSPBranch.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BSPTree::BSPTree(const std::vector<BSPNode_Ptr>& nodes)
:	m_nodes(nodes)
{
	index_leaves();
}

//#################### PUBLIC METHODS ####################
/**
Returns the number of empty leaves in the tree.

@return	As stated
*/
int BSPTree::empty_leaf_count() const
{
	return m_emptyLeafCount;
}

/**
Finds the index of the leaf in which the specified point resides.

@param p	The point
*/
int BSPTree::find_leaf_index(const Vector3d& p) const
{
	BSPNode_Ptr cur = root();
	while(!cur->is_leaf())
	{
		BSPBranch *branch = cur->as_branch();
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

	BSPLeaf *leaf = cur->as_leaf();
	return leaf->leaf_index();
}

/**
Returns the leaf with leaf index n.

@param n			The leaf index
@return				As stated
@throws Exception	If the leaf index is out of range
*/
const BSPLeaf *BSPTree::leaf(int n) const
{
	int leafCount = static_cast<int>(m_leaves.size());
	if(n >= 0 && n < leafCount) return m_leaves[n];
	else throw Exception("Leaf index out of range");
}

/**
Determines whether or not there is line-of-sight between p1 and p2.

@param p1	A point in the world
@param p2	Another point in the world
@return		true, if there is line-of-sight between the points, or false otherwise
*/
bool BSPTree::line_of_sight(const Vector3d& p1, const Vector3d& p2) const
{
	return line_of_sight_sub(p1, p2, root());
}

BSPTree_Ptr BSPTree::load_postorder_text(std::istream& is)
{
	std::string line;
	std::getline(is, line);
	int nodeCount;
	try							{ nodeCount = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The BSP node count is not a number"); }

	std::vector<BSPNode_Ptr> nodes(nodeCount);

	int n = 0;
	while(n < nodeCount && std::getline(is, line))
	{
		typedef boost::char_separator<char> sep;
		typedef boost::tokenizer<sep> tokenizer;

		tokenizer tok(line, sep(" "));
		std::vector<std::string> tokens(tok.begin(), tok.end());
		size_t tokenCount = tokens.size();
		if(tokenCount < 2) throw Exception("Bad BSP node: " + lexical_cast<std::string,int>(n));

		if(tokens[1] == "E")
		{
			if(tokenCount < 6 || tokens[4] != "[" || tokens[tokenCount-1] != "]")
				throw Exception("Bad empty leaf node: " + lexical_cast<std::string,int>(n));

			int polyCount;
			try							{ polyCount = lexical_cast<int,std::string>(tokens[3]); }
			catch(bad_lexical_cast&)	{ throw Exception("The leaf polygon count is not a number: " + lexical_cast<std::string,int>(n)); }

			std::vector<int> polyIndices;

			for(size_t i=5; i<tokenCount-1; ++i)
			{
				int polyIndex;
				try							{ polyIndex = lexical_cast<int,std::string>(tokens[i]); }
				catch(bad_lexical_cast&)	{ throw Exception("A polygon index is not a number in leaf: " + lexical_cast<std::string,int>(n)); }

				polyIndices.push_back(polyIndex);
			}

			nodes[n] = BSPLeaf::make_empty_leaf(n, polyIndices);
		}
		else if(tokens[1] == "S")
		{
			nodes[n] = BSPLeaf::make_solid_leaf(n);
		}
		else if(tokens[1] == "B")
		{
			if(tokenCount != 11 || tokens[5] != "(" || tokens[10] != ")")
				throw Exception("Bad branch node: " + lexical_cast<std::string,int>(n));

			int leftIndex, rightIndex;
			double a, b, c, d;
			try
			{
				leftIndex = lexical_cast<int,std::string>(tokens[2]);
				rightIndex = lexical_cast<int,std::string>(tokens[3]);
				a = lexical_cast<double,std::string>(tokens[6]);
				b = lexical_cast<double,std::string>(tokens[7]);
				c = lexical_cast<double,std::string>(tokens[8]);
				d = lexical_cast<double,std::string>(tokens[9]);
			}
			catch(bad_lexical_cast&)	{ throw Exception("One of the values was not a number in branch node: " + lexical_cast<std::string,int>(n)); }

			BSPNode_Ptr left = nodes[leftIndex];
			BSPNode_Ptr right = nodes[rightIndex];
			if(left && right)
			{
				Plane_Ptr splitter(new Plane(Vector3d(a,b,c), d));
				nodes[n] = BSPNode_Ptr(new BSPBranch(n, splitter, left, right));
				left->set_parent(nodes[n].get());
				right->set_parent(nodes[n].get());
			}
			else throw Exception("The BSP nodes are not stored in postorder: the child nodes for this branch have not yet been loaded");
		}

		++n;
	}

	return BSPTree_Ptr(new BSPTree(nodes));
}

void BSPTree::output_postorder_text(std::ostream& os) const
{
	os << m_nodes.size() << '\n';
	root()->output_postorder_text(os);
}

BSPNode_Ptr BSPTree::root() const
{
	return m_nodes.back();
}

//#################### PRIVATE METHODS ####################
void BSPTree::index_leaves()
{
	index_specific_leaves(root(), false);
	m_emptyLeafCount = static_cast<int>(m_leaves.size());
	index_specific_leaves(root(), true);
}

void BSPTree::index_specific_leaves(const BSPNode_Ptr& node, bool solidFlag)
{
	if(node->is_leaf())
	{
		BSPLeaf *leaf = node->as_leaf();
		if(leaf->is_solid() == solidFlag)
		{
			int nextLeaf = static_cast<int>(m_leaves.size());
			leaf->set_leaf_index(nextLeaf);
			m_leaves.push_back(leaf);
		}
	}
	else
	{
		const BSPBranch *branch = node->as_branch();
		index_specific_leaves(branch->left(), solidFlag);
		index_specific_leaves(branch->right(), solidFlag);
	}
}

bool BSPTree::line_of_sight_sub(const Vector3d& p1, const Vector3d& p2, const BSPNode_Ptr& node) const
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
