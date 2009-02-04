/***
 * hesperus: IPhysicsComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IPHYSICSCOMPONENT
#define H_HESP_IPHYSICSCOMPONENT

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

class IPhysicsComponent
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~IPhysicsComponent() {}

	//#################### PUBLIC METHODS ####################
public:
	virtual double mass() const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<IPhysicsComponent> IPhysicsComponent_Ptr;

}

#endif
