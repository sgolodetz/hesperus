/***
 * hesperus: ExplicitLayout.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_GUI_EXPLICITLAYOUT
#define H_HESP_GUI_EXPLICITLAYOUT

#include "Component.h"

namespace hesp {

class ExplicitLayout
{
	//#################### PUBLIC METHODS ####################
public:
	void add(const Component_Ptr& component, const Extents& extents);

};

}

#endif
