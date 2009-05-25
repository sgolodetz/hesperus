/***
 * hesperus: FirstPersonCamera.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FirstPersonCamera.h"

#include <source/level/objects/components/ICmpAABBBounds.h>
#include <source/level/objects/components/ICmpOrientation.h>
#include <source/level/objects/components/ICmpPosition.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
FirstPersonCamera::FirstPersonCamera(const ObjectID& viewer, const ObjectManager_Ptr& objectManager)
:	m_viewer(viewer), m_objectManager(objectManager)
{}

//#################### PUBLIC METHODS ####################
Vector3d FirstPersonCamera::at() const
{
	return m_at;
}

Vector3d FirstPersonCamera::eye() const
{
	return m_eye;
}

bool FirstPersonCamera::is_inside_viewer() const
{
	return true;
}

Vector3d FirstPersonCamera::look() const
{
	return m_look;
}

Vector3d FirstPersonCamera::up() const
{
	return Vector3d(0,0,1);
}

void FirstPersonCamera::update()
{
	ICmpAABBBounds_Ptr cmpBounds = m_objectManager->get_component(m_viewer, cmpBounds);
	ICmpOrientation_Ptr cmpOrientation = m_objectManager->get_component(m_viewer, cmpOrientation);
	ICmpPosition_Ptr cmpPosition = m_objectManager->get_component(m_viewer, cmpPosition);

	const Vector3d& pos = cmpPosition->position();
	m_look = cmpOrientation->nuv_axes()->n();

	// Calculate the viewer's eye position and where they're looking at.
	const AABB3d& aabb = m_objectManager->aabbs()[cmpBounds->cur_aabb_index()];
	m_eye = pos + Vector3d(0,0,aabb.maximum().z * 0.9);
	m_at = m_eye + m_look;
}

}
