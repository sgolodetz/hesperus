/***
 * hesperus: ICmpHealth.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPHEALTH
#define H_HESP_ICMPHEALTH

#include <source/level/objects/base/ObjectComponent.h>

namespace hesp {

class ICmpHealth : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual int health() const = 0;
	virtual int max_health() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Health"; }
	static std::string static_group_type()	{ return "Health"; }

	std::string own_type() const			{ return "Health"; }
	static std::string static_own_type()	{ return "Health"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpHealth> ICmpHealth_Ptr;
typedef shared_ptr<const ICmpHealth> ICmpHealth_CPtr;

}

#endif
