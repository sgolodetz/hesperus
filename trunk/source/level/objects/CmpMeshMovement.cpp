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

//#################### STATIC FACTORY METHODS ####################
IComponent_Ptr CmpMeshMovement::create(const Properties&)
{
	return IComponent_Ptr(new CmpMeshMovement);
}

//#################### PUBLIC METHODS ####################
int CmpMeshMovement::cur_nav_poly_index() const
{
	return m_curNavPolyIndex;
}

CmpMeshMovement::Traversal_CPtr CmpMeshMovement::cur_traversal() const
{
	return m_curTraversal;
}

double CmpMeshMovement::run_speed() const
{
	// FIXME: This should be loaded in.
	return 10.0;	// in units/s
}

void CmpMeshMovement::set_cur_nav_poly_index(int curNavPolyIndex)
{
	m_curNavPolyIndex = curNavPolyIndex;
}

void CmpMeshMovement::set_cur_traversal(const Traversal_Ptr& curTraversal)
{
	m_curTraversal = curTraversal;
}

double CmpMeshMovement::walk_speed() const
{
	// FIXME: This should be loaded in.
	return 5.0;		// in units/s
}

}
