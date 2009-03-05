/***
 * hesperus: Model.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MODEL
#define H_HESP_MODEL

#include "Mesh.h"
#include "Skeleton.h"

namespace hesp {

class Model
{
	//#################### PRIVATE VARIABLES ####################
private:
	Mesh_Ptr m_mesh;
	Skeleton_Ptr m_skeleton;

	//#################### CONSTRUCTORS ####################
public:
	Model(const Mesh_Ptr& mesh, const Skeleton_Ptr& skeleton);
};

}

#endif
