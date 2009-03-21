/***
 * hesperus: RBTQuaternion.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_RBTQUATERNION
#define H_HESP_RBTQUATERNION

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "Quaternion.h"

namespace hesp {

/**
This class represents a rigid-body transform as a quaternion and translation vector.
*/
class RBTQuaternion
{
	//#################### PRIVATE VARIABLES ####################
private:
	Quaternion m_rot;
	Vector3d m_trans;

	// TODO
};

//#################### TYPEDEFS ####################
typedef shared_ptr<RBTQuaternion> RBTQuaternion_Ptr;
typedef shared_ptr<const RBTQuaternion> RBTQuaternion_CPtr;

}

#endif
