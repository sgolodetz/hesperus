/***
 * hesperus: ExplicitLayout.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_EXPLICITLAYOUT
#define H_HESP_EXPLICITLAYOUT

#include <vector>

#include "LaidOutComponent.h"

namespace hesp {

/**
This class allows a Container to be laid out explicitly by specifying
explicit extents for each component added.
*/
class ExplicitLayout
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<LaidOutComponent> m_components;

	//#################### PUBLIC METHODS ####################
public:
	void add(Component *component, const Extents& extents);
	void add(const Component_Ptr& component, const Extents& extents);
	std::vector<LaidOutComponent> fit(const Extents& extents) const;
};

}

#endif
