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
BSPNode_Ptr BSPLeaf::make_empty_leaf(int index, const std::vector<int>& polygonIndices)
{
	BSPLeaf *leaf = new BSPLeaf(index);
	leaf->m_isSolid = false;
	leaf->m_polygonIndices = polygonIndices;
	return BSPNode_Ptr(leaf);
}

BSPNode_Ptr BSPLeaf::make_solid_leaf(int index)
{
	BSPLeaf *leaf = new BSPLeaf(index);
	leaf->m_isSolid = true;
	return BSPNode_Ptr(leaf);
}

//#################### PUBLIC METHODS ####################
const BSPBranch *BSPLeaf::as_branch() const					{ return NULL; }
const BSPLeaf *BSPLeaf::as_leaf() const						{ return this; }
bool BSPLeaf::is_leaf() const								{ return true; }
bool BSPLeaf::is_solid() const								{ return m_isSolid; }

void BSPLeaf::output_postorder_text(std::ostream& os) const
{
	os << m_index;
	if(m_isSolid) os << " S ";
	else os << " L ";
	os << m_parent->index();

	if(!m_isSolid)
	{
		size_t polyCount = m_polygonIndices.size();
		os << ' ' << polyCount << " [ ";
		for(size_t i=0; i<polyCount; ++i)
		{
			os << m_polygonIndices[i] << ' ';
		}
		os << "]";
	}

	os << '\n';
}

const std::vector<int>& BSPLeaf::polygon_indices() const	{ return m_polygonIndices; }

}
