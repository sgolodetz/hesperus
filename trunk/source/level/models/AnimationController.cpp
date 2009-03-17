/***
 * hesperus: AnimationController.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AnimationController.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
AnimationController::AnimationController(const Mesh_Ptr& mesh, const Skeleton_Ptr& skeleton)
:	m_mesh(mesh), m_skeleton(skeleton), m_state(AS_REST), m_animationTime(0)
{}

//#################### PUBLIC METHODS ####################
void AnimationController::request_animation(const std::string& newAnimation)
{
	switch(m_state)
	{
		case AS_REST:
		{
			// Start the new animation immediately as there's no existing animation from which to transition.
			m_state = AS_PLAY;
			m_animation = newAnimation;
			m_animationTime = 0;
			break;
		}
		case AS_PLAY:
		{
			// Start a transition between the current animation and the new one.
			m_state = AS_TRANSITION;
			m_animation = newAnimation;
			m_animationTime = 0;
			m_transitionTime = 0;
			break;
		}
		case AS_TRANSITION:
		{
			// Start transitioning towards a new animation instead of the one to which we were previously transitioning.
			m_animation = newAnimation;
			m_transitionTime = 0;
			break;
		}
	}
}

void AnimationController::update(int milliseconds)
{
	update_skeleton(milliseconds);
	m_mesh->skin(m_skeleton);
}

//#################### PRIVATE METHODS ####################
void AnimationController::update_skeleton(int milliseconds)
{
	switch(m_state)
	{
		case AS_REST:
		{
			m_skeleton->set_rest_pose();
			break;
		}
		case AS_PLAY:
		{
			// Interpolate between the current pose and an appropriate keyframe of the current animation.
			// TODO
			break;
		}
		case AS_TRANSITION:
		{
			// Interpolate between the current pose and the start keyframe of the new animation.
			// TODO
			break;
		}
	}

	// NYI
	throw 23;
}

}
