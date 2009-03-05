/***
 * hesperus: Skeleton.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SKELETON
#define H_HESP_SKELETON

#include "Animation.h"
#include "BoneConfiguration.h"

namespace hesp {

class Skeleton
{
	//#################### PRIVATE VARIABLES ####################
private:
	BoneConfiguration_Ptr m_boneConfiguration;
	std::vector<Matrix44> m_baseBoneMatrices;
	std::map<std::string,Animation_Ptr> m_animations;

	// TODO
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Skeleton> Skeleton_Ptr;
typedef shared_ptr<const Skeleton> Skeleton_CPtr;

}

#endif
