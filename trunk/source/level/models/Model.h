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

class Model
{
	//#################### PRIVATE VARIABLES ####################
private:
	AnimationController_Ptr m_animController;
	Mesh_Ptr m_mesh;
	Skeleton_Ptr m_skeleton;

	//#################### CONSTRUCTORS ####################
public:
	Model(const Mesh_Ptr& mesh, const Skeleton_Ptr& skeleton);

	//#################### PUBLIC METHODS ####################
public:
	const AnimationController_Ptr& anim_controller() const;
	void render() const;
	void update(int milliseconds);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Model> Model_Ptr;
typedef shared_ptr<const Model> Model_CPtr;

}

#endif
