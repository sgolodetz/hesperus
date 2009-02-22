/***
 * hesperus: NavComponent.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavComponent.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
NavComponent::NavComponent()
:	m_curLinkIndex(-1), m_curNavPolyIndex(-1)
{}

//#################### PUBLIC METHODS ####################
int NavComponent::cur_link_index() const
{
	return m_curLinkIndex;
}

int NavComponent::cur_nav_poly_index() const
{
	return m_curNavPolyIndex;
}

void NavComponent::set_cur_link_index(int curLinkIndex)
{
	m_curLinkIndex = curLinkIndex;
}

void NavComponent::set_cur_nav_poly_index(int curNavPolyIndex)
{
	m_curNavPolyIndex = curNavPolyIndex;
}

}
