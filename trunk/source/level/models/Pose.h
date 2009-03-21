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
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Pose> Pose_Ptr;
typedef shared_ptr<const Pose> Pose_CPtr;

}

#endif
