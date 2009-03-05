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
	// TODO: Extract and store base bone matrices.
}

}
