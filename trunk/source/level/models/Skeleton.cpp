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
	set_pose(get_rest_pose());
	build_to_bone_matrices();
}

//#################### PUBLIC METHODS ####################
const Animation_Ptr& Skeleton::animation(const std::string& name) const
{
	std::map<std::string,Animation_Ptr>::const_iterator it = m_animations.find(name);
	if(it != m_animations.end())
	{
		return it->second;
	}
	else throw Exception("There is no animation named " + name);
}

const BoneConfiguration_Ptr& Skeleton::bone_configuration() const
{
	return m_boneConfiguration;
}

Pose_Ptr Skeleton::get_rest_pose() const
{
	int boneCount = m_boneConfiguration->bone_count();
	std::vector<RBTMatrix_Ptr> boneMatrices(boneCount, RBTMatrix::identity());
	return Pose_Ptr(new Pose(boneMatrices));
}

bool Skeleton::has_animation(const std::string& name) const
{
	return m_animations.find(name) != m_animations.end();
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

void Skeleton::set_pose(const Pose_CPtr& pose, const std::map<std::string,PoseModifier>& modifiers)
{
	const std::vector<RBTMatrix_Ptr>& boneMatrices = pose->bone_matrices();
	int boneCount = m_boneConfiguration->bone_count();
	for(int i=0; i<boneCount; ++i)
	{
		Bone_Ptr bone = m_boneConfiguration->bones(i);
		
		if(bone->name() == "root" && bone->parent() != NULL)
		{
			// If this is the root bone of an attached model, its absolute matrix should be
			// the same as the bone to which it's attached: to ensure this, we explicitly
			// set its relative matrix to the identity matrix. Note that only the root bone
			// of each model should be called "root", or this will cause problems.
			bone->relative_matrix() = RBTMatrix::identity();
		}
		else
		{
			const RBTMatrix_Ptr& baseRot = bone->base_rotation();
			RBTMatrix_Ptr rot(new RBTMatrix(*boneMatrices[i]));
			(*rot)(0,3) = (*rot)(1,3) = (*rot)(2,3) = 0;

			RBTMatrix_Ptr relMatrix = baseRot * rot;

			const Vector3d& basePos = bone->base_position();
			Vector3d trans((*boneMatrices[i])(0,3), (*boneMatrices[i])(1,3), (*boneMatrices[i])(2,3));
			(*relMatrix)(0,3) = basePos.x + trans.x;
			(*relMatrix)(1,3) = basePos.y + trans.y;
			(*relMatrix)(2,3) = basePos.z + trans.z;

			bone->relative_matrix() = relMatrix;
		}
	}
	update_absolute_bone_matrices(modifiers);
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

void Skeleton::calculate_absolute_bone_matrix(const Bone_Ptr& bone, const std::map<std::string,PoseModifier>& modifiers)
{
	Bone_Ptr parent = bone->parent();
	if(parent)
	{
		if(!parent->absolute_matrix()) calculate_absolute_bone_matrix(parent, modifiers);
		bone->absolute_matrix() = parent->absolute_matrix() * bone->relative_matrix();
	}
	else
	{
		bone->absolute_matrix() = RBTMatrix::copy(bone->relative_matrix());
	}

	// Handle any pose modifiers applied to this bone.
	std::map<std::string,PoseModifier>::const_iterator it = modifiers.find(bone->name());
	if(it != modifiers.end())
	{
		// This bone has a pose modifier applied to it, so update the absolute matrix accordingly.
		const PoseModifier& modifier = it->second;

		// Calculate the required rotation axis in the frame of the bone.
		Vector3d transformedAxis = bone->absolute_matrix()->inverse()->apply_to_vector(modifier.axis);

		// Construct an appropriate rotation matrix and use it to post-multiply the bone's absolute matrix.
		// Note that if we needed the relative matrix again, we'd need to post-multiply that as well, but
		// in practice this isn't necessary.
		RBTMatrix_Ptr rot = RBTMatrix::from_axis_angle_translation(transformedAxis, modifier.angle, Vector3d(0,0,0));
		bone->absolute_matrix() *= rot;
	}
}

void Skeleton::update_absolute_bone_matrices(const std::map<std::string,PoseModifier>& modifiers)
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
		calculate_absolute_bone_matrix(m_boneConfiguration->bones(i), modifiers);
	}
}

}
