/***
 * hesperus: ICmpPosition.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPPOSITION
#define H_HESP_ICMPPOSITION

#include <source/level/objects/base/IComponent.h>
#include <source/math/vectors/Vector3.h>

namespace hesp {

class ICmpPosition : public IComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual const Vector3d& position() const = 0;
	virtual void set_position(const Vector3d& position) = 0;

	//#################### PUBLIC METHODS ####################
public:
	static std::string static_type()	{ return "Position"; }
	std::string type() const			{ return "Position"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpPosition> ICmpPosition_Ptr;
typedef shared_ptr<const ICmpPosition> ICmpPosition_CPtr;

}

#endif
