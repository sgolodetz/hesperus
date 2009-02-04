/***
 * hesperus: IPhysicsComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IPHYSICSCOMPONENT
#define H_HESP_IPHYSICSCOMPONENT

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/vectors/Vector3.h>

namespace hesp {

class IPhysicsComponent
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~IPhysicsComponent() {}

	//#################### PUBLIC METHODS ####################
public:
	virtual double mass() const = 0;
	virtual void set_velocity(const Vector3d& velocity) = 0;
	virtual const Vector3d& velocity() const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<IPhysicsComponent> IPhysicsComponent_Ptr;

}

#endif
