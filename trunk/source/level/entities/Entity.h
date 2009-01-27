/***
 * hesperus: Entity.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITY
#define H_HESP_ENTITY

#include <iosfwd>

namespace hesp {

class Entity
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~Entity() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void save(std::ostream& os) const = 0;
};

}

#endif
