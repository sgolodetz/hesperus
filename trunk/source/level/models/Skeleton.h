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
	BoneConfiguration m_boneConfiguration;
	std::vector<Matrix44> m_baseBoneMatrices;
	std::map<std::string,Animation> m_animations;

	// TODO
};

}

#endif
