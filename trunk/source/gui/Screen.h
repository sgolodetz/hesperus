/***
 * hesperus: Screen.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_GUI_SCREEN
#define H_HESP_GUI_SCREEN

#include "Container.h"
#include "ExplicitLayout.h"

namespace hesp {

class Screen : public Container<ExplicitLayout>
{
	//#################### SINGLETON IMPLEMENTATION ####################
private:
	Screen() {}
	Screen(const Screen&);
	Screen& operator=(const Screen&);
public:
	static Screen& instance();

	//#################### PUBLIC METHODS ####################
public:
	void fit(const Extents& extents);
	void render() const;
	void set_ortho_viewport(const Extents& extents);
};

}

#endif
