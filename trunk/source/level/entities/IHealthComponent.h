/***
 * hesperus: IHealthComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IHEALTHCOMPONENT
#define H_HESP_IHEALTHCOMPONENT

#include "IEntityComponent.h"

namespace hesp {

class IHealthComponent : public IEntityComponent
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~IHealthComponent() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual int health() const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<IHealthComponent> IHealthComponent_Ptr;

}

#endif
