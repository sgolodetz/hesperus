/***
 * hesperus: ExplicitLayout.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "ExplicitLayout.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Adds a component to the layout and specifies explicit extents for it.

@param component	The component to add
@param extents		Explicit extents for the component (relative to its container)
*/
void ExplicitLayout::add(Component *component, const Extents& extents)
{
	add(Component_Ptr(component), extents);
}

/**
Adds a component to the layout and specifies explicit extents for it.

@param component	The component to add
@param extents		Explicit extents for the component (relative to its container)
*/
void ExplicitLayout::add(const Component_Ptr& component, const Extents& extents)
{
	m_components.push_back(LaidOutComponent(component, extents));
}

/**
Fits the components to the specified container extents and returns them.

@param extents	The extents of the container for which this object is the layout
@return			A collection of laid-out components to be put into the container
*/
std::vector<LaidOutComponent> ExplicitLayout::fit(const Extents& extents) const
{
	std::vector<LaidOutComponent> components(m_components);

	for(std::vector<LaidOutComponent>::iterator it=components.begin(), iend=components.end(); it!=iend; ++it)
	{
		// TODO:	Check that the component fits within the container's extents, and clip
		//			its own extents if not.

		it->extents = it->extents.translate(extents.left(), extents.top());
	}

	return components;
}

}
