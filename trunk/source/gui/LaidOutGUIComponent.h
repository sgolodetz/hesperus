/***
 * hesperus: LaidOutGUIComponent.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_LAIDOUTGUICOMPONENT
#define H_HESP_LAIDOUTGUICOMPONENT

#include "GUIComponent.h"

namespace hesp {

struct LaidOutGUIComponent
{
	//#################### PUBLIC VARIABLES ####################
	GUIComponent_Ptr component;
	Extents extents;

	//#################### CONSTRUCTORS ####################
	LaidOutGUIComponent(const GUIComponent_Ptr& component_, const Extents& extents_)
	:	component(component_), extents(extents_)
	{}
};

}

#endif
