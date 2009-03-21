/***
 * hesperus: Skeleton.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Skeleton.h"

#include <source/exceptions/Exception.h>
#include <source/ogl/WrappedGL.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Skeleton::Skeleton(const BoneConfiguration_Ptr& boneConfiguration, const std::map<std::string,Animation_Ptr>& animations)
:	m_boneConfiguration(boneConfiguration), m_animations(animations)
{
	set_rest_pose();
	build_to_bone_matrices();
}

//#################### PUBLIC METHODS ####################
const BoneConfiguration_Ptr& Skeleton::bone_configuration() const
{
	return m_boneConfiguration;
}

void Skeleton::render_bones() const
{
	int boneCount = m_boneConfiguration->bone_count();
	for(int i=0; i<boneCount; ++i)
	{
		const Bone_Ptr& bone = m_boneConfiguration->bones(i);
		Vector3d pos = bone->position();
		Vector3d orient = bone->orientation();
		Vector3d end = pos + orient * 0.5;

		glBegin(GL_LINES);
			glColor3d(1,0,0);	glVertex3d(pos.x, pos.y, pos.z);
			glColor3d(0,1,0);	glVertex3d(end.x, end.y, end.z);
		glEnd();

#if 0
		Bone_Ptr parent = bone->parent();
		if(bone->name() == "foot.l") parent = m_boneConfiguration->bones("lower_leg.l");
		if(bone->name() == "foot.r") parent = m_boneConfiguration->bones("lower_leg.r");
		if(parent)
		{
			Vector3d parentPos = parent->position();
			glColor3d(1,1,1);
			glBegin(GL_LINES);
				glVertex3d(pos.x, pos.y, pos.z);
				glVertex3d(parentPos.x, parentPos.y, parentPos.z);
			glEnd();
		}
#endif
	}
}

void Skeleton::select_keyframe(const std::string& animationName, int keyframeIndex)
{
	std::map<std::string,Animation_Ptr>::const_iterator it = m_animations.find(animationName);
	if(it != m_animations.end())
	{
		const Animation_Ptr& animation = it->second;
		const Pose_Ptr& keyframe = animation->keyframes(keyframeIndex);
		specify_relative_bone_matrices(keyframe->bone_matrices());
	}
	else throw Exception("There is no animation named " + animationName);
}

void Skeleton::specify_relative_bone_matrices(const std::vector<RBTMatrix_Ptr>& boneMatrices)
{
	int boneCount = m_boneConfiguration->bone_count();
	for(int i=0; i<boneCount; ++i)
	{
		const RBTMatrix_Ptr& baseRot = m_boneConfiguration->bones(i)->base_rotation();
		RBTMatrix_Ptr rot(new RBTMatrix(*boneMatrices[i]));
		(*rot)(0,3) = (*rot)(1,3) = (*rot)(2,3) = 0;

		RBTMatrix_Ptr relMatrix = baseRot * rot;

		const Vector3d& basePos = m_boneConfiguration->bones(i)->base_position();
		Vector3d trans((*boneMatrices[i])(0,3), (*boneMatrices[i])(1,3), (*boneMatrices[i])(2,3));
		(*relMatrix)(0,3) = basePos.x + trans.x;
		(*relMatrix)(1,3) = basePos.y + trans.y;
		(*relMatrix)(2,3) = basePos.z + trans.z;

		m_boneConfiguration->bones(i)->relative_matrix() = relMatrix;
	}
	update_absolute_bone_matrices();
}

void Skeleton::set_rest_pose()
{
	int boneCount = m_boneConfiguration->bone_count();
	std::vector<RBTMatrix_Ptr> boneMatrices(boneCount, RBTMatrix::identity());
	specify_relative_bone_matrices(boneMatrices);
}

const RBTMatrix_Ptr& Skeleton::to_bone_matrix(int i) const
{
	return m_toBoneMatrices[i];
}

//#################### PRIVATE METHODS ####################
void Skeleton::build_to_bone_matrices()
{
	int boneCount = m_boneConfiguration->bone_count();
	m_toBoneMatrices.resize(boneCount);

	for(int i=0; i<boneCount; ++i)
	{
		const RBTMatrix_Ptr& fromBoneMatrix = m_boneConfiguration->bones(i)->absolute_matrix();
		m_toBoneMatrices[i] = fromBoneMatrix->inverse();
	}
}

void Skeleton::calculate_absolute_bone_matrix(const Bone_Ptr& bone)
{
	Bone_Ptr parent = bone->parent();
	if(parent)
	{
		if(!parent->absolute_matrix()) calculate_absolute_bone_matrix(parent);
		bone->absolute_matrix() = parent->absolute_matrix() * bone->relative_matrix();
	}
	else
	{
		bone->absolute_matrix() = bone->relative_matrix();
	}
}

void Skeleton::update_absolute_bone_matrices()
{
	int boneCount = m_boneConfiguration->bone_count();

	// Step 1: Clear all the existing absolute bone matrices.
	for(int i=0; i<boneCount; ++i)
	{
		m_boneConfiguration->bones(i)->absolute_matrix().reset();
	}

	// Step 2: Calculate the new absolute bone matrices.
	for(int i=0; i<boneCount; ++i)
	{
		calculate_absolute_bone_matrix(m_boneConfiguration->bones(i));
	}
}

}
