/***
 * hesperus: CmpPhysics.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPPHYSICS
#define H_HESP_CMPPHYSICS

#include "ICmpPhysics.h"

namespace hesp {

class CmpPhysics : public ICmpPhysics
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_velocity;

	//#################### PUBLIC METHODS ####################
public:
	void set_velocity(const Vector3d& velocity);
	const Vector3d& velocity() const;
};

}

#endif
