/***
 * hesperus: ICmpDirectMovement.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPDIRECTMOVEMENT
#define H_HESP_ICMPDIRECTMOVEMENT

#include <list>

#include <source/level/objects/base/ObjectComponent.h>
#include <source/level/trees/OnionUtil.h>

namespace hesp {

class ICmpDirectMovement : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual const std::list<OnionUtil::Transition_Ptr>& recent_transitions() const = 0;
	virtual void update_recent_transitions(const OnionUtil::Transition_Ptr& transition) = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Movement"; }
	static std::string static_group_type()	{ return "Movement"; }

	std::string own_type() const			{ return "DirectMovement"; }
	static std::string static_own_type()	{ return "DirectMovement"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpDirectMovement> ICmpDirectMovement_Ptr;
typedef shared_ptr<const ICmpDirectMovement> ICmpDirectMovement_CPtr;

}

#endif
