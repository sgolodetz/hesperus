/***
 * hesperus: BipedChangePoseCommand.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BipedChangePoseCommand.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BipedChangePoseCommand::BipedChangePoseCommand(const Entity_Ptr& biped)
:	m_biped(biped)
{}

//#################### PUBLIC METHODS ####################
void BipedChangePoseCommand::execute(const OnionTree_Ptr& tree, int milliseconds)
{
	ICameraComponent_Ptr camComponent = m_biped->camera_component();
	Vector3d source = camComponent->camera().position();

	ICollisionComponent_Ptr colComponent = m_biped->collision_component();
	int mapIndex = colComponent->pose();
	colComponent->set_pose(1 - mapIndex);
	Vector3d dest = camComponent->camera().position();

	OnionTree::Transition transition = tree->find_first_transition(mapIndex, source, dest);
	if(transition.classifier != OnionTree::RAY_EMPTY)
	{
		colComponent->set_pose(1 - mapIndex);
	}
}

}
