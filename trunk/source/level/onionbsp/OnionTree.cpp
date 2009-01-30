/***
 * hesperus: OnionTree.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "OnionTree.h"

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>
#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
OnionTree::OnionTree(const std::vector<OnionNode_Ptr>& nodes, int mapCount)
:	m_nodes(nodes), m_mapCount(mapCount)
{
	index_leaves();
}

//#################### PUBLIC METHODS ####################
OnionTree::Transition OnionTree::find_first_transition(int mapIndex, const Vector3d& source, const Vector3d& dest) const
{
	return find_first_transition_sub(mapIndex, source, dest, root());
}

const OnionLeaf *OnionTree::leaf(int n) const
{
	return m_leaves[n];
}

OnionTree_Ptr OnionTree::load_postorder_text(std::istream& is)
{
	std::string line;

	std::getline(is, line);
	int mapCount;
	try							{ mapCount = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The map count is not a number"); }

	std::getline(is, line);
	int nodeCount;
	try							{ nodeCount = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The onion node count is not a number"); }

	std::vector<OnionNode_Ptr> nodes(nodeCount);

	int n = 0;
	while(n < nodeCount && std::getline(is, line))
	{
		typedef boost::char_separator<char> sep;
		typedef boost::tokenizer<sep> tokenizer;

		tokenizer tok(line, sep(" "));
		std::vector<std::string> tokens(tok.begin(), tok.end());
		size_t tokenCount = tokens.size();
		if(tokenCount < 2) throw Exception("Bad onion node: " + lexical_cast<std::string,int>(n));

		if(tokens[1] == "B")
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

			OnionNode_Ptr left = nodes[leftIndex];
			OnionNode_Ptr right = nodes[rightIndex];
			if(left && right)
			{
				Plane_Ptr splitter(new Plane(Vector3d(a,b,c), d));
				nodes[n] = OnionNode_Ptr(new OnionBranch(n, splitter, left, right));
			}
			else throw Exception("The onion nodes are not stored in postorder: the child nodes for this branch have not yet been loaded");
		}
		else
		{
			// We're dealing with an onion leaf.
			if(tokenCount < 8 || tokens[1] != "(" || tokens[3] != ")" || tokens[6] != "[" || tokens[tokenCount-1] != "]")
				throw Exception("Bad leaf node: " + lexical_cast<std::string,int>(n));

			if(tokens[2].length() != mapCount)
				throw Exception("Bad leaf solidity descriptor: " + lexical_cast<std::string,int>(n));

			// Construct the solidity descriptor.
			boost::dynamic_bitset<> solidityDescriptor(mapCount);
			for(size_t i=0, len=tokens[2].length(); i<len; ++i)
			{
				if(tokens[2][i] == '0') solidityDescriptor.set(i, false);
				else if(tokens[2][i] == '1') solidityDescriptor.set(i, true);
				else throw Exception("Bad leaf solidity descriptor: " + lexical_cast<std::string,int>(n));
			}

			// Read in any polygon indices.
			int polyCount;
			try							{ polyCount = lexical_cast<int,std::string>(tokens[5]); }
			catch(bad_lexical_cast&)	{ throw Exception("The leaf polygon count is not a number: " + lexical_cast<std::string,int>(n)); }

			std::vector<int> polyIndices;

			for(size_t i=7; i<tokenCount-1; ++i)
			{
				int polyIndex;
				try							{ polyIndex = lexical_cast<int,std::string>(tokens[i]); }
				catch(bad_lexical_cast&)	{ throw Exception("A polygon index is not a number in leaf: " + lexical_cast<std::string,int>(n)); }

				polyIndices.push_back(polyIndex);
			}

			nodes[n].reset(new OnionLeaf(n, solidityDescriptor, polyIndices));
		}

		++n;
	}

	return OnionTree_Ptr(new OnionTree(nodes, mapCount));
}

int OnionTree::map_count() const
{
	return m_mapCount;
}

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
OnionTree::Transition OnionTree::find_first_transition_sub(int mapIndex, const Vector3d& source, const Vector3d& dest, const OnionNode_Ptr& node) const
{
	if(node->is_leaf())
	{
		const OnionLeaf *leaf = node->as_leaf();
		if(leaf->is_solid(mapIndex)) return Transition(RAY_SOLID);
		else return Transition(RAY_EMPTY);
	}

	const OnionBranch *branch = node->as_branch();
	const OnionNode_Ptr& left = branch->left();
	const OnionNode_Ptr& right = branch->right();

	Plane_CPtr splitter = branch->splitter();
	PlaneClassifier cpSource, cpDest;
	switch(classify_linesegment_against_plane(source, dest, *splitter, cpSource, cpDest))
	{
		case CP_BACK:
		{
			return find_first_transition_sub(mapIndex, source, dest, right);
		}
		case CP_COPLANAR:
		{
			// TODO
			break;
		}
		case CP_FRONT:
		{
			return find_first_transition_sub(mapIndex, source, dest, left);
		}
		case CP_STRADDLE:
		{
			Vector3d mid = determine_linesegment_intersection_with_plane(source, dest, *splitter).first;
			OnionNode_Ptr near, far;
			if(cpSource == CP_FRONT)
			{
				near = left;
				far = right;
			}
			else
			{
				near = right;
				far = left;
			}

			// Search for a transition on the near side of the plane: if we find one, that's the first transition.
			Transition trNear = find_first_transition_sub(mapIndex, source, mid, near);
			if(trNear.location) return trNear;

			// Search for a transition on the far side of the plane.
			Transition trFar = find_first_transition_sub(mapIndex, mid, dest, far);

			switch(trFar.classifier)
			{
				case RAY_EMPTY:
				{
					// If both sides are empty, there's no transition, otherwise there's a solid -> empty transition
					// at the point where the ray intersects the current split plane.
					if(trNear.classifier == RAY_EMPTY) return Transition(RAY_EMPTY);
					else return Transition(RAY_TRANSITION_SE, Vector3d_Ptr(new Vector3d(mid)), splitter);
				}
				case RAY_SOLID:
				{
					// If both sides are solid, there's no transition, otherwise there's an empty -> solid transition
					// at the point where the ray intersects the current split plane.
					if(trNear.classifier == RAY_EMPTY) return Transition(RAY_TRANSITION_ES, Vector3d_Ptr(new Vector3d(mid)), splitter);
					else return Transition(RAY_SOLID);
				}
				case RAY_TRANSITION_ES:
				{
					// If the near side is empty, the first transition is the empty -> solid one on the far side.
					// Otherwise, there's a nearer solid -> empty transition on the current split plane.
					if(trNear.classifier == RAY_EMPTY) return trFar;
					else return Transition(RAY_TRANSITION_SE, Vector3d_Ptr(new Vector3d(mid)), splitter);
				}
				case RAY_TRANSITION_SE:
				{
					// If the near side is solid, the first transition is the solid -> empty one on the far side.
					// Otherwise, there's a nearer empty -> solid transition on the current split plane.
					if(trNear.classifier == RAY_SOLID) return trFar;
					else return Transition(RAY_TRANSITION_ES, Vector3d_Ptr(new Vector3d(mid)), splitter);
				}
				default:
				{
					break;
				}
			}

			break;
		}
	}

	// We'll never actually get here: this is just for safety's sake.
	throw Exception("Unexpectedly reached the end of OnionTree::find_first_transition_sub()");
}

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
