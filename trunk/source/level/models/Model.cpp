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
const Mesh_Ptr& Model::mesh() const
{
	return m_mesh;
}

const Skeleton_Ptr& Model::skeleton() const
{
	return m_skeleton;
}

}
