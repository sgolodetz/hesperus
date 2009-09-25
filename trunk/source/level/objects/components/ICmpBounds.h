/***
 * hesperus: ICmpBounds.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPBOUNDS
#define H_HESP_ICMPBOUNDS

#include <source/level/objects/base/ObjectComponent.h>

namespace hesp {

class ICmpBounds : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual const std::string& bounds_group() const = 0;
	virtual const std::string& posture() const = 0;
	virtual void set_posture(const std::string& posture) = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Bounds"; }
	static std::string static_group_type()	{ return "Bounds"; }

	std::string own_type() const			{ return "Bounds"; }
	static std::string static_own_type()	{ return "Bounds"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpBounds> ICmpBounds_Ptr;
typedef shared_ptr<const ICmpBounds> ICmpBounds_CPtr;

}

#endif
