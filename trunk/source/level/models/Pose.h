/***
 * hesperus: Pose.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_POSE
#define H_HESP_POSE

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/matrices/RBTMatrix.h>

namespace hesp {

//#################### TYPEDEFS ####################
typedef shared_ptr<class Pose> Pose_Ptr;
typedef shared_ptr<const class Pose> Pose_CPtr;

class Pose
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<RBTMatrix_Ptr> m_boneMatrices;

	//#################### CONSTRUCTORS ####################
public:
	Pose(const std::vector<RBTMatrix_Ptr>& boneMatrices);

	//#################### PUBLIC METHODS ####################
public:
	const std::vector<RBTMatrix_Ptr>& bone_matrices() const;
	static Pose_Ptr interpolate(const Pose_Ptr& lhs, const Pose_Ptr& rhs, double t);
};

}

#endif
