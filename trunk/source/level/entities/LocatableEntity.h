/***
 * hesperus: LocatableEntity.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LOCATABLEENTITY
#define H_HESP_LOCATABLEENTITY

#include <source/math/vectors/Vector3.h>

namespace hesp {

/**
This class represents a locatable entity (i.e. the entity has a position in space).
*/
class LocatableEntity
{
	//#################### PROTECTED VARIABLES ####################
protected:
	Vector3d m_position;

	//#################### CONSTRUCTORS ####################
public:
	LocatableEntity(const Vector3d& position)
	:	m_position(position)
	{}

	//#################### DESTRUCTOR ####################
public:
	virtual ~LocatableEntity() {}

	//#################### PUBLIC METHODS ####################
public:
	const Vector3d& position() const
	{
		return m_position;
	}
};

}

#endif
