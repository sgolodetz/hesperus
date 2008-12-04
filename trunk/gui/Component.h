/***
 * hesperus: Component.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_GUI_COMPONENT
#define H_HESP_GUI_COMPONENT

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "Extents.h"

namespace hesp {

class Component
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~Component() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void fit(const Extents& extents) = 0;
	virtual void render() const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Component> Component_Ptr;
typedef shared_ptr<const Component> Component_CPtr;

}

#endif
