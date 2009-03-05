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
	std::string m_name;
	Matrix44_Ptr m_absoluteMatrix;
	Matrix44_Ptr m_relativeMatrix;
	Bone_Ptr m_parent;

	//#################### CONSTRUCTORS ####################
public:
	Bone(const std::string& name, const Vector3d& position, const Vector3d& rotationAxis, double rotationAngle);

	//#################### PUBLIC METHODS ####################
public:
	const std::string& name() const;
	void set_parent(const Bone_Ptr& parent);
};

}

#endif
