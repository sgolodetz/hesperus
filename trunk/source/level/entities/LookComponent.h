/***
 * hesperus: LookComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LOOKCOMPONENT
#define H_HESP_LOOKCOMPONENT

#include <source/io/FieldIO.h>
#include "ILookComponent.h"

namespace hesp {

class LookComponent : public ILookComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_look;

	//#################### CONSTRUCTORS ####################
public:
	LookComponent(std::istream& is)
	{
		m_look = FieldIO::read_typed_field<Vector3d>(is, "Look");
	}

	//#################### PUBLIC METHODS ####################
public:
	const Vector3d& look() const
	{
		return m_look;
	}

	void save(std::ostream& os) const
	{
		FieldIO::write_typed_field(os, "Look", m_look);
	}
};

}

#endif
