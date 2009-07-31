/***
 * hesperus: Model.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Model.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Model::Model(const Mesh_Ptr& mesh, const Skeleton_Ptr& skeleton)
:	m_mesh(mesh), m_skeleton(skeleton)
{}

//#################### PUBLIC METHODS ####################
void Model::apply_pose_to_skeleton(const AnimationController_CPtr& animController)
{
	m_skeleton->set_pose(animController->get_pose());
}

void Model::attach_to_parent(const Model_Ptr& parent, const std::string& parentBoneName)
{
	Bone_Ptr childBone = m_skeleton->bone_configuration()->bones("root");
	Bone_Ptr parentBone = parent->skeleton()->bone_configuration()->bones(parentBoneName);
	childBone->set_parent(parentBone);
}

void Model::detach_from_parent()
{
	Bone_Ptr childBone = m_skeleton->bone_configuration()->bones("root");
	childBone->set_parent(Bone_Ptr());
}

void Model::render() const
{
	m_mesh->skin(m_skeleton);
	m_mesh->render();
}

const Skeleton_Ptr& Model::skeleton() const
{
	return m_skeleton;
}

}
