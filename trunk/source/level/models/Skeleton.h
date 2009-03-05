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
	std::vector<Matrix44_CPtr> m_baseBoneMatrices;
	std::map<std::string,Animation_Ptr> m_animations;

	//#################### CONSTRUCTORS ####################
public:
	Skeleton(const BoneConfiguration_Ptr& boneConfiguration, const std::map<std::string,Animation_Ptr>& animations);

	//#################### PUBLIC METHODS ####################
public:
	void render_bones() const;
	void select_keyframe(const std::string& animationName, int keyframe);
	void specify_relative_bone_matrices(const std::vector<Matrix44_CPtr>& boneMatrices);

	//#################### PRIVATE METHODS ####################
private:
	void update_absolute_bone_matrices();
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Skeleton> Skeleton_Ptr;
typedef shared_ptr<const Skeleton> Skeleton_CPtr;

}

#endif
