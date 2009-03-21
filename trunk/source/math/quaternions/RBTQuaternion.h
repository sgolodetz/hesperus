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

//#################### TYPEDEFS ####################
typedef shared_ptr<class RBTQuaternion> RBTQuaternion_Ptr;
typedef shared_ptr<const class RBTQuaternion> RBTQuaternion_CPtr;

/**
This class represents a rigid-body transform as a quaternion and translation vector.
*/
class RBTQuaternion
{
	//#################### PRIVATE VARIABLES ####################
private:
	Quaternion m_rotation;
	Vector3d m_translation;

	//#################### CONSTRUCTORS ####################
private:
	RBTQuaternion();

	//#################### STATIC FACTORY METHODS ####################
public:
	static RBTQuaternion_Ptr from_axis_angle_translation(Vector3d axis, double angle, const Vector3d& translation);

	//#################### PUBLIC METHODS ####################
public:
	Vector3d apply(const Vector3d& p) const;
};

}

#endif
