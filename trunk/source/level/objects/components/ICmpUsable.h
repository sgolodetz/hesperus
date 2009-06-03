/***
 * hesperus: ICmpUsable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPUSABLE
#define H_HESP_ICMPUSABLE

#include <source/level/objects/base/IComponent.h>

namespace hesp {

/**
Objects with an ICmpUsable component can be used.
*/
class ICmpUsable : public IComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	/**
	Returns the usable's group (e.g. sword, pistol, tool, etc.)
	*/
	virtual std::string usable_group() const = 0;

	/**
	Uses the usable (e.g. fires the weapon, uses the lockpick, etc.)
	*/
	virtual void use() = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Usable"; }
	static std::string static_group_type()	{ return "Usable"; }

	std::string own_type() const			{ return "Usable"; }
	static std::string static_own_type()	{ return "Usable"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpUsable> ICmpUsable_Ptr;
typedef shared_ptr<const ICmpUsable> ICmpUsable_CPtr;

}

#endif
