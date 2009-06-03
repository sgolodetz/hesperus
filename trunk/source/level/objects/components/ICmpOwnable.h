/***
 * hesperus: ICmpOwnable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPOWNABLE
#define H_HESP_ICMPOWNABLE

#include <source/level/objects/base/IComponent.h>

namespace hesp {

/**
Objects with an ICmpOwnable component can be owned by other objects.
*/
class ICmpOwnable : public IComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void clear_owner() = 0;
	virtual const ObjectID& owner() const = 0;
	virtual void set_owner(const ObjectID& owner) = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Ownable"; }
	static std::string static_group_type()	{ return "Ownable"; }

	std::string own_type() const			{ return "Ownable"; }
	static std::string static_own_type()	{ return "Ownable"; }
};

}

#endif
