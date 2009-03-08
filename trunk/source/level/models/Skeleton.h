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
	std::map<std::string,Animation_Ptr> m_animations;

	// In order to do mesh skinning, we need to be able to move points into
	// the (rest) coordinate frame of each bone. These matrices fulfil that
	// role.
	std::vector<Matrix44_Ptr> m_toBoneMatrices;

	//#################### CONSTRUCTORS ####################
public:
	Skeleton(const BoneConfiguration_Ptr& boneConfiguration, const std::map<std::string,Animation_Ptr>& animations);

	//#################### PUBLIC METHODS ####################
public:
	void render_bones() const;
	void select_keyframe(const std::string& animationName, int keyframeIndex);
	void specify_relative_bone_matrices(const std::vector<Matrix44_Ptr>& boneMatrices);
	void set_rest_pose();

	//#################### PRIVATE METHODS ####################
private:
	void build_to_bone_matrices();
	void calculate_absolute_bone_matrix(const Bone_Ptr& bone);
	static Matrix44_Ptr invert_rigid_body_transform(const Matrix44_Ptr& mat);
	void update_absolute_bone_matrices();
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Skeleton> Skeleton_Ptr;
typedef shared_ptr<const Skeleton> Skeleton_CPtr;

}

#endif
