/***
 * hesperus: ICmpPhysics.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPPHYSICS
#define H_HESP_ICMPPHYSICS

#include <source/math/vectors/Vector3.h>
#include "IComponent.h"

namespace hesp {

class ICmpPhysics : public IComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void set_velocity(const Vector3d& velocity) = 0;
	virtual const Vector3d& velocity() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	static std::string static_type()	{ return "Physics"; }
	std::string type() const			{ return "Physics"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpPhysics> ICmpPhysics_Ptr;
typedef shared_ptr<const ICmpPhysics> ICmpPhysics_CPtr;

}

#endif
