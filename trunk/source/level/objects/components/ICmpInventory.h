/***
 * hesperus: ICmpInventory.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPINVENTORY
#define H_HESP_ICMPINVENTORY

#include <source/level/objects/base/IComponent.h>

namespace hesp {

/**
Objects with an ICmpInventory component can own other objects.
*/
class ICmpInventory : public IComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void add_consumables(const std::string& type, int amount) = 0;
	virtual void add_object(const ObjectID& objectID) = 0;
	virtual void destroy_consumables(const std::string& type, int amount) = 0;
	virtual void remove_object(const ObjectID& objectID) = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Inventory"; }
	static std::string static_group_type()	{ return "Inventory"; }

	std::string own_type() const			{ return "Inventory"; }
	static std::string static_own_type()	{ return "Inventory"; }
};

}

#endif
