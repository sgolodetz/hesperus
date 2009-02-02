/***
 * hesperus: NavComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVCOMPONENT
#define H_HESP_NAVCOMPONENT

#include "INavComponent.h"

namespace hesp {

class NavComponent : public INavComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_curNavPolyIndex;	// the index of the nav polygon in which the centre of the entity's AABB currently resides (-1 if unknown)

	//#################### CONSTRUCTORS ####################
public:
	NavComponent() : m_curNavPolyIndex(-1) {}

	//#################### PUBLIC METHODS ####################
public:
	int cur_nav_poly_index() const
	{
		return m_curNavPolyIndex;
	}

	void set_cur_nav_poly_index(int curNavPolyIndex)
	{
		m_curNavPolyIndex = curNavPolyIndex;
	}
};

}

#endif
