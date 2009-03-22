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
	// Note: If we're already transitioning, we keep transitioning, just towards a new target.
	if(m_state == AS_REST) m_state = AS_PLAY;
	else if(m_state == AS_PLAY) m_state = AS_TRANSITION;

	m_animation = newAnimation;
	m_animationTime = 0;
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
			m_skeleton->set_pose(m_skeleton->get_rest_pose());
			break;
		}
		case AS_PLAY:
		{
			// Interpolate between the current pose and an appropriate keyframe of the current animation.

			m_animationTime += milliseconds;
			// TODO: If the animation time has been longer than the (scaled) length of the animation, loop back round.

			int keyframeIndex;
			double t;

			// TODO: Calculate keyframe index and t.
			throw 23;

			Pose_Ptr curPose = m_skeleton->get_current_pose();
			Pose_Ptr newPose = m_skeleton->get_keyframe(m_animation, keyframeIndex);
			m_skeleton->set_pose(Pose::interpolate(curPose, newPose, t));
			break;
		}
		case AS_TRANSITION:
		{
			Pose_Ptr curPose = m_skeleton->get_current_pose();
			Pose_Ptr newPose;
			double t;

			if(m_animation != "<rest>")
			{
				// NYI: Calculate t.
				throw 23;

				// If the new animation isn't the rest animation, transition to its start keyframe.
				newPose = m_skeleton->get_keyframe(m_animation, 0);
			}
			else
			{
				// NYI: Calculate t.
				throw 23;

				newPose = m_skeleton->get_rest_pose();
			}

			m_animationTime += milliseconds;
			if(m_animationTime >= TRANSITION_TIME)
			{
				// The transition is over, switch back to normal play.
				m_state = AS_PLAY;
				m_animationTime = 0;
				m_skeleton->set_pose(newPose);
			}
			else
			{
				// Still transitioning: interpolate between the current pose and the new pose.
				m_skeleton->set_pose(Pose::interpolate(curPose, newPose, t));
			}
			break;
		}
	}

	// NYI
	throw 23;
}

}
