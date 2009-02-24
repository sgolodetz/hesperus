/***
 * hesperus: IPhysicsComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IPHYSICSCOMPONENT
#define H_HESP_IPHYSICSCOMPONENT

#include <iosfwd>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/vectors/Vector3.h>

namespace hesp {

class IPhysicsComponent
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~IPhysicsComponent() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual double mass() const = 0;
	virtual double run_speed() const = 0;
	virtual void save(std::ostream& os) const = 0;
	virtual void set_velocity(const Vector3d& velocity) = 0;
	virtual const Vector3d& velocity() const = 0;
	virtual double walk_speed() const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<IPhysicsComponent> IPhysicsComponent_Ptr;

}

#endif
