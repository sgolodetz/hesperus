/***
 * hesperus: CmpPosition.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPPOSITION
#define H_HESP_CMPPOSITION

#include "ICmpPosition.h"

namespace hesp {

class CmpPosition : public ICmpPosition
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_position;

	//#################### PUBLIC METHODS ####################
public:
	const Vector3d& position() const;
	void set_position(const Vector3d& position);
};

}

#endif
