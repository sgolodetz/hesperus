/***
 * hesperus: Skeleton.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Skeleton.h"

#include <source/exceptions/Exception.h>

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

//#################### PUBLIC METHODS ####################
void Skeleton::render_bones() const
{
	// NYI
	throw 23;
}

void Skeleton::select_keyframe(const std::string& animationName, int keyframeIndex)
{
	std::map<std::string,Animation_Ptr>::const_iterator it = m_animations.find(animationName);
	if(it != m_animations.end())
	{
		const Animation_Ptr& animation = it->second;
		const Keyframe_Ptr& keyframe = animation->keyframes(keyframeIndex);
		specify_relative_bone_matrices(keyframe->bone_matrices());
	}
	else throw Exception("There is no animation named " + animationName);
}

void Skeleton::specify_relative_bone_matrices(const std::vector<Matrix44_CPtr>& boneMatrices)
{
	// NYI
	throw 23;
}

//#################### PRIVATE METHODS ####################
void Skeleton::update_absolute_bone_matrices()
{
	// NYI
	throw 23;
}

}
