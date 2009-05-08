/***
 * hesperus: CmpMeshMovement.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpMeshMovement.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpMeshMovement::CmpMeshMovement()
:	m_curNavPolyIndex(-1)
{}

//#################### PUBLIC METHODS ####################
int CmpMeshMovement::cur_nav_poly_index() const
{
	return m_curNavPolyIndex;
}

CmpMeshMovement::Traversal_CPtr CmpMeshMovement::cur_traversal() const
{
	return m_curTraversal;
}

void CmpMeshMovement::set_cur_nav_poly_index(int curNavPolyIndex)
{
	m_curNavPolyIndex = curNavPolyIndex;
}

void CmpMeshMovement::set_cur_traversal(const Traversal_Ptr& curTraversal)
{
	m_curTraversal = curTraversal;
}

}
