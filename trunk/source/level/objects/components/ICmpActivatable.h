/***
 * hesperus: ICmpActivatable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPACTIVATABLE
#define H_HESP_ICMPACTIVATABLE

#include <source/level/objects/base/IComponent.h>

namespace hesp {

class ICmpActivatable : public IComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void activated_by(const ObjectID& activator) = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Activatable"; }
	static std::string static_group_type()	{ return "Activatable"; }

	std::string own_type() const			{ return "Activatable"; }
	static std::string static_own_type()	{ return "Activatable"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpActivatable> ICmpActivatable_Ptr;
typedef shared_ptr<const ICmpActivatable> ICmpActivatable_CPtr;

}

#endif