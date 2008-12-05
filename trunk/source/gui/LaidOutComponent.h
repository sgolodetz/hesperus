/***
 * hesperus: LaidOutComponent.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_GUI_LAIDOUTCOMPONENT
#define H_HESP_GUI_LAIDOUTCOMPONENT

#include "Component.h"

namespace hesp {

struct LaidOutComponent
{
	//#################### PUBLIC VARIABLES ####################
	Component_Ptr component;
	Extents extents;

	//#################### CONSTRUCTORS ####################
	LaidOutComponent(const Component_Ptr& component_, const Extents& extents_)
	:	component(component_), extents(extents_)
	{}
};

}

#endif
