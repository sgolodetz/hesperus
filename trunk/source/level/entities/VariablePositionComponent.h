/***
 * hesperus: VariablePositionComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_VARIABLEPOSITIONCOMPONENT
#define H_HESP_VARIABLEPOSITIONCOMPONENT

#include <source/io/FieldIO.h>
#include "IPositionComponent.h"

namespace hesp {

class VariablePositionComponent : public IPositionComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_position;

	//#################### CONSTRUCTORS ####################
public:
	VariablePositionComponent(std::istream& is)
	{
		m_position = FieldIO::read_typed_field<Vector3d>(is, "Position");
	}

	//#################### PUBLIC METHODS ####################
public:
	const Vector3d& position() const
	{
		return m_position;
	}

	void save(std::ostream& os) const
	{
		FieldIO::write_typed_field(os, "Position", m_position);
	}

	void set_position(const Vector3d& position)
	{
		m_position = position;
	}
};

}

#endif
