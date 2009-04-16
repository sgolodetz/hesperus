/***
 * hesperus: BipedTurnCommand.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BipedTurnCommand.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BipedTurnCommand::BipedTurnCommand(const Entity_Ptr& biped, int mouseMotionX, int mouseMotionY)
:	m_biped(biped), m_mouseMotionX(mouseMotionX), m_mouseMotionY(mouseMotionY)
{}

//#################### PUBLIC METHODS ####################
void BipedTurnCommand::execute(const std::vector<AABB3d>& aabbs, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree,
							   const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds)
{
	Camera& camera = m_biped->camera();

	const double CAMERA_SPEED = 15.0;

	// For left-right motion, rotate about the (0,0,1) axis.
	camera.rotate(Vector3d(0,0,1), -m_mouseMotionX * CAMERA_SPEED * (milliseconds/1000.0));

	// For up-down motion, rotate about the camera's u axis. Note that this rotation is clamped internally by the camera
	// to prevent the n vector being parallel to (0,0,1).
	camera.rotate(camera.u(), m_mouseMotionY * CAMERA_SPEED * (milliseconds/1000.0));
}

}
