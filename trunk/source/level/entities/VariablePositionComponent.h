/***
 * hesperus: VariablePositionComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_VARIABLEPOSITIONCOMPONENT
#define H_HESP_VARIABLEPOSITIONCOMPONENT

#include "IPositionComponent.h"

namespace hesp {

class VariablePositionComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_position;

	//#################### CONSTRUCTORS ####################
public:
	VariablePositionComponent(const Vector3d& position)
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
		m_position = position;
	}
};

}

#endif
