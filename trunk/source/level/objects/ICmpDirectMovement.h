/***
 * hesperus: ICmpDirectMovement.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPDIRECTMOVEMENT
#define H_HESP_ICMPDIRECTMOVEMENT

#include <list>

#include <source/level/trees/OnionUtil.h>
#include "IComponent.h"

namespace hesp {

class ICmpDirectMovement : public IComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual const std::list<OnionUtil::Transition_Ptr>& recent_transitions() const = 0;
	virtual void update_recent_transitions(const OnionUtil::Transition_Ptr& transition) = 0;

	//#################### PUBLIC METHODS ####################
public:
	static std::string static_type()	{ return "Movement"; }
	std::string type() const			{ return "Movement"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpDirectMovement> ICmpDirectMovement_Ptr;
typedef shared_ptr<const ICmpDirectMovement> ICmpDirectMovement_CPtr;

}

#endif
