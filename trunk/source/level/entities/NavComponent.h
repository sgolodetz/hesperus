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
	int m_curLinkIndex;		// the index of the nav link which the entity is currently traversing (-1 if none)
	int m_curNavPolyIndex;	// the index of the nav polygon in which the centre of the entity's AABB currently resides (-1 if unknown)

	//#################### CONSTRUCTORS ####################
public:
	NavComponent();

	//#################### PUBLIC METHODS ####################
public:
	int cur_link_index() const;
	int cur_nav_poly_index() const;
	void set_cur_link_index(int curLinkIndex);
	void set_cur_nav_poly_index(int curNavPolyIndex);
};

}

#endif
