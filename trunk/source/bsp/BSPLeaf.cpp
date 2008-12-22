/***
 * hesperus: BSPLeaf.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "BSPLeaf.h"

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

#include <source/exceptions/Exception.h>

namespace hesp {

//#################### STATIC FACTORY METHODS ####################
BSPNode_Ptr BSPLeaf::make_empty_leaf(const std::vector<int>& polygonIndices)
{
	BSPLeaf *leaf = new BSPLeaf;
	leaf->m_isSolid = false;
	leaf->m_polygonIndices = polygonIndices;
	return BSPNode_Ptr(leaf);
}

BSPNode_Ptr BSPLeaf::make_solid_leaf()
{
	BSPLeaf *leaf = new BSPLeaf;
	leaf->m_isSolid = true;
	return BSPNode_Ptr(leaf);
}

//#################### PUBLIC METHODS ####################
BSPBranch *BSPLeaf::as_branch()								{ return NULL; }
const BSPBranch *BSPLeaf::as_branch() const					{ return NULL; }
BSPLeaf *BSPLeaf::as_leaf()									{ return this; }
const BSPLeaf *BSPLeaf::as_leaf() const						{ return this; }
bool BSPLeaf::is_leaf() const								{ return true; }
bool BSPLeaf::is_solid() const								{ return m_isSolid; }
const std::vector<int>& BSPLeaf::polygon_indices() const	{ return m_polygonIndices; }

}
