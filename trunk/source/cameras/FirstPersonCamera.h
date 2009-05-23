/***
 * hesperus: FirstPersonCamera.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_FIRSTPERSONCAMERA
#define H_HESP_FIRSTPERSONCAMERA

#include <source/level/objects/base/ObjectManager.h>
#include "Camera.h"

namespace hesp {

class FirstPersonCamera : public Camera
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_viewer;
	ObjectManager_Ptr m_objectManager;

	//#################### CONSTRUCTORS ####################
public:
	FirstPersonCamera(const ObjectID& viewer, const ObjectManager_Ptr& objectManager);

	//#################### PUBLIC METHODS ####################
public:
	Vector3d set_view() const;
};

}

#endif
