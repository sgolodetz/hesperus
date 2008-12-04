/***
 * hesperus: Container.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_GUI_CONTAINER
#define H_HESP_GUI_CONTAINER

#include <vector>

#include "LaidOutComponent.h"

namespace hesp {

template <typename Layout>
class Container : public Component
{
	//#################### PRIVATE VARIABLES ####################
private:
	Layout m_layout;
	std::vector<LaidOutComponent> m_components;

	//#################### PUBLIC METHODS ####################
public:
	void fit(const Extents& extents);
	Layout& layout();
	void render() const;
};

}

#include "Container.tpp"

#endif
