/***
 * hesperus: FixedPositionComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_FIXEDPOSITIONCOMPONENT
#define H_HESP_FIXEDPOSITIONCOMPONENT

#include "IPositionComponent.h"

namespace hesp {

class FixedPositionComponent : public IPositionComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_position;

	//#################### CONSTRUCTORS ####################
public:
	FixedPositionComponent(const Vector3d& position)
	:	m_position(position)
	{}

	//#################### PUBLIC METHODS ####################
public:
	const Vector3d& position() const
	{
		return m_position;
	}

	void set_position(const Vector3d& position)
	{
		// No-op
	}
};

}

#endif
