/***
 * hesperus: OrientedEntity.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ORIENTEDENTITY
#define H_HESP_ORIENTEDENTITY

#include <source/math/vectors/Vector3.h>

namespace hesp {

/**
This class represents an oriented entity (i.e. the entity has a look vector).
*/
class OrientedEntity
{
	//#################### PROTECTED VARIABLES ####################
protected:
	Vector3d m_look;

	//#################### CONSTRUCTORS ####################
public:
	OrientedEntity(const Vector3d& look)
	:	m_look(look)
	{}

	//#################### DESTRUCTOR ####################
public:
	virtual ~OrientedEntity() {}

	//#################### PUBLIC METHODS ####################
public:
	const Vector3d& look() const
	{
		return m_look;
	}
};

}

#endif
