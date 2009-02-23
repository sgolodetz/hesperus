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
	int m_curNavPolyIndex;			// the index of the nav polygon in which the centre of the entity's AABB currently resides (-1 if unknown)
	Traversal_Ptr m_curTraversal;	// the details of the current nav link traversal (NULL if not currently traversing a link)

	//#################### CONSTRUCTORS ####################
public:
	NavComponent();

	//#################### PUBLIC METHODS ####################
public:
	int cur_nav_poly_index() const;
	Traversal_Ptr cur_traversal() const;
	void set_cur_nav_poly_index(int curNavPolyIndex);
	void set_cur_traversal(const Traversal_Ptr& traversal);
};

}

#endif
