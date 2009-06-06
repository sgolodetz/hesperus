/***
 * hesperus: ObjectComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTCOMPONENT
#define H_HESP_OBJECTCOMPONENT

#include "IComponent.h"

namespace hesp {

class ObjectComponent : public IComponent
{
	//#################### PROTECTED METHODS ####################
protected:
	template <typename C> void check_dependency() const;
};

}

#include "ObjectComponent.tpp"

#endif
