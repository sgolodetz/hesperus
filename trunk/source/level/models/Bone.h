/***
 * hesperus: Bone.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BONE
#define H_HESP_BONE

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/matrices/Matrix44.h>
#include <source/math/vectors/Vector3.h>

namespace hesp {

//#################### TYPEDEFS ####################
typedef shared_ptr<class Bone> Bone_Ptr;
typedef shared_ptr<const class Bone> Bone_CPtr;

class Bone
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_id;
	std::string m_name;
	Matrix44 m_absoluteMatrix;
	Matrix44 m_relativeMatrix;
	Bone_Ptr m_parent;

	//#################### CONSTRUCTORS ####################
public:
	Bone(int id, const std::string& name, const Vector3d& position, double rotationAngle, const Vector3d& rotationAxis);
};

}

#endif
