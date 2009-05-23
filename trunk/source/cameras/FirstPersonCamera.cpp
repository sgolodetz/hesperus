/***
 * hesperus: FirstPersonCamera.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FirstPersonCamera.h"

#include <source/ogl/WrappedGL.h>
#include <gl/glu.h>

#include <source/level/objects/components/ICmpCollision.h>
#include <source/level/objects/components/ICmpOrientation.h>
#include <source/level/objects/components/ICmpPosition.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
FirstPersonCamera::FirstPersonCamera(const ObjectID& viewer, const ObjectManager_Ptr& objectManager)
:	m_viewer(viewer), m_objectManager(objectManager)
{}

//#################### PUBLIC METHODS ####################
bool FirstPersonCamera::is_inside_viewer() const
{
	return true;
}

Vector3d FirstPersonCamera::set_view() const
{
	ICmpCollision_Ptr cmpCollision = m_objectManager->get_component(m_viewer, cmpCollision);
	ICmpOrientation_Ptr cmpOrientation = m_objectManager->get_component(m_viewer, cmpOrientation);
	ICmpPosition_Ptr cmpPosition = m_objectManager->get_component(m_viewer, cmpPosition);

	const Vector3d& pos = cmpPosition->position();
	const Vector3d& look = cmpOrientation->nuv_axes()->n();

	// Calculate the viewer's eye position and where they're looking at.
	const AABB3d& aabb = m_objectManager->aabbs()[cmpCollision->cur_aabb_index()];
	Vector3d eye = pos + Vector3d(0,0,aabb.maximum().z * 0.9);
	Vector3d at = eye + look;

	// Set the camera accordingly.
	gluLookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, 0, 0, 1);

	return eye;
}

}
