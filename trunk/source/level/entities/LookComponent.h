/***
 * hesperus: LookComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LOOKCOMPONENT
#define H_HESP_LOOKCOMPONENT

#include "ILookComponent.h"

namespace hesp {

class LookComponent : public ILookComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_look;

	//#################### CONSTRUCTORS ####################
public:
	LookComponent(const Vector3d& look)
	:	m_look(look)
	{}

	//#################### PUBLIC METHODS ####################
public:
	const Vector3d& look() const
	{
		return m_look;
	}
};

}

#endif
