/***
 * hesperus: IVisibilityComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IVISIBILITYCOMPONENT
#define H_HESP_IVISIBILITYCOMPONENT

#include "IEntityComponent.h"

namespace hesp {

class IVisibilityComponent : public IEntityComponent
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~IVisibilityComponent() {}
};

//#################### TYPEDEFS ####################
typedef shared_ptr<IVisibilityComponent> IVisibilityComponent_Ptr;

}

#endif
