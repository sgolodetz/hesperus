/***
 * hesperus: Model.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MODEL
#define H_HESP_MODEL

#include "AnimationController.h"
#include "Mesh.h"
#include "Skeleton.h"

namespace hesp {

//#################### TYPEDEFS ####################
typedef shared_ptr<class Model> Model_Ptr;
typedef shared_ptr<const class Model> Model_CPtr;

class Model
{
	//#################### PRIVATE VARIABLES ####################
private:
	Mesh_Ptr m_mesh;
	Skeleton_Ptr m_skeleton;

	//#################### CONSTRUCTORS ####################
public:
	Model(const Mesh_Ptr& mesh, const Skeleton_Ptr& skeleton);

	//#################### PUBLIC METHODS ####################
public:
	void attach_to_parent(const Model_Ptr& parent, const std::string& parentBoneName);
	void detach_from_parent();
	void render(const AnimationController_Ptr& animController) const;
	const Skeleton_Ptr& skeleton() const;
};

}

#endif
