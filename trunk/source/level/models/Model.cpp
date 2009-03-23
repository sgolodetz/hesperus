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
void Model::render(const AnimationController_Ptr& animController) const
{
	m_skeleton->set_pose(animController->get_pose());
	m_mesh->skin(m_skeleton);
	m_mesh->render();
}

const Skeleton_Ptr& Model::skeleton() const
{
	return m_skeleton;
}

}
