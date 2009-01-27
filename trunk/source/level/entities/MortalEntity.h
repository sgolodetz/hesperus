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
	//#################### PROTECTED VARIABLES ####################
protected:
	int m_health;

	//#################### CONSTRUCTORS ####################
public:
	MortalEntity(int health)
	:	m_health(health) {}

	//#################### DESTRUCTOR ####################
public:
	virtual ~MortalEntity() {}

	//#################### PUBLIC METHODS ####################
public:
	int health() const
	{
		return m_health;
	}
};

}

#endif
