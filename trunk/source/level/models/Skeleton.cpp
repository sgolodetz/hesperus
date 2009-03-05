/***
 * hesperus: Skeleton.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Skeleton.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Skeleton::Skeleton(const BoneConfiguration_Ptr& boneConfiguration, const std::map<std::string,Animation_Ptr>& animations)
:	m_boneConfiguration(boneConfiguration), m_animations(animations)
{
	// Extract and store base bone matrices.
	int boneCount = boneConfiguration->bone_count();
	m_baseBoneMatrices.resize(boneCount);
	for(int i=0; i<boneCount; ++i)
	{
		m_baseBoneMatrices[i] = boneConfiguration->bones(i)->relative_matrix();
	}
}

}
