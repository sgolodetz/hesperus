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
		const Keyframe_Ptr& keyframe = animation->keyframes(keyframeIndex);
		specify_relative_bone_matrices(keyframe->bone_matrices());
	}
	else throw Exception("There is no animation named " + animationName);
}

void Skeleton::specify_relative_bone_matrices(const std::vector<Matrix44_Ptr>& boneMatrices)
{
	int boneCount = m_boneConfiguration->bone_count();
	for(int i=0; i<boneCount; ++i)
	{
		const Matrix44_Ptr& baseRot = m_boneConfiguration->bones(i)->base_rotation();
		Matrix44_Ptr rot(new Matrix44(*boneMatrices[i]));
		(*rot)(0,3) = (*rot)(1,3) = (*rot)(2,3) = 0;

		Matrix44_Ptr relMatrix = baseRot * rot;

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
	std::vector<Matrix44_Ptr> boneMatrices(boneCount, Matrix44::identity());
	specify_relative_bone_matrices(boneMatrices);
}

//#################### PRIVATE METHODS ####################
void Skeleton::build_to_bone_matrices()
{
	int boneCount = m_boneConfiguration->bone_count();
	m_toBoneMatrices.resize(boneCount);

	for(int i=0; i<boneCount; ++i)
	{
		const Matrix44_Ptr& fromBoneMatrix = m_boneConfiguration->bones(i)->absolute_matrix();
		m_toBoneMatrices[i] = invert_rigid_body_transform(fromBoneMatrix);
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

Matrix44_Ptr invert_rigid_body_transform(const Matrix44_Ptr& mat)
{
	/*
	[ux vx wx tx] -1   ( [1 0 0 tx]   [ux vx wx 0] ) -1
	[uy vy wy ty]      ( [0 1 0 ty]   [uy vy wy 0] )
	[uz vz wz tz]    = ( [0 0 1 tz] * [uz vz wz 0] )
	[ 0  0  0  1]      ( [0 0 0  1]   [ 0  0  0 1] )

		               [ux vx wx 0] -1   [1 0 0 tx] -1
			           [uy vy wy 0]      [0 1 0 ty]
				     = [ux vz wz 0]    * [0 0 1 tz]
					   [ 0  0  0 1]      [0 0 0  1]

	                   [ux uy uz 0]   [1 0 0 -tx]
		               [vx vy vz 0]   [0 1 0 -ty]
			         = [wx wy wz 0] * [0 0 1 -tz]
				       [ 0  0  0 1]   [0 0 0  1 ]

	                   [ux uy uz -ux*tx-uy*ty-uz*tz]
		               [vx vy vz -vx*tx-vy*ty-vz*tz]
			         = [wx wy wz -wx*tx-wy*ty-wz*tz]
				       [ 0  0  0          1        ]

	                   [ux uy uz -dot(u,t)]
		               [vx vy vz -dot(v,t)]
			         = [wx wy wz -dot(w,t)]
				       [ 0  0  0     1    ]
	*/

	const Matrix44& m = *mat;

	Vector3d u(m(0,0), m(1,0), m(2,0));
	Vector3d v(m(0,1), m(1,1), m(2,1));
	Vector3d w(m(0,2), m(1,2), m(2,2));
	Vector3d t(m(0,3), m(1,3), m(2,3));

	Matrix44_Ptr inv = Matrix44::zeros();
	Matrix44& i = *inv;

	i(0,0) = u.x;	i(0,1) = u.y;	i(0,2) = u.z;	i(0,3) = -u.dot(t);
	i(1,0) = v.x;	i(1,1) = v.y;	i(1,2) = v.z;	i(1,3) = -v.dot(t);
	i(2,0) = w.x;	i(2,1) = w.y;	i(2,2) = w.z;	i(2,3) = -w.dot(t);
													i(3,3) = 1;

	return inv;
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
