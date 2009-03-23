/***
 * hesperus: Model.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Model.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Model::Model(const Mesh_Ptr& mesh, const Skeleton_Ptr& skeleton)
:	m_mesh(mesh), m_skeleton(skeleton), m_animController(new AnimationController(skeleton))
{}

//#################### PUBLIC METHODS ####################
const AnimationController_Ptr& Model::anim_controller() const
{
	return m_animController;
}

void Model::render() const
{
	m_skeleton->set_pose(m_animController->get_pose());
	m_mesh->skin(m_skeleton);
	m_mesh->render();
}

void Model::update(int milliseconds)
{
	m_animController->update(milliseconds);
}

}
