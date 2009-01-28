/***
 * hesperus: MortalEntity.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MORTALENTITY
#define H_HESP_MORTALENTITY

#include "Entity.h"

namespace hesp {

/**
This class represents a mortal entity (i.e. this entity can die, so it maintains a health value).
*/
class MortalEntity : virtual public Entity
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~MortalEntity() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual int health() const = 0;
};

}

#endif
