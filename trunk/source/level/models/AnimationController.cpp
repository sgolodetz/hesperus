/***
 * hesperus: AnimationController.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AnimationController.h"

#include <cmath>

namespace hesp {

//#################### CONSTRUCTORS ####################
AnimationController::AnimationController(const Mesh_Ptr& mesh, const Skeleton_Ptr& skeleton)
:	m_mesh(mesh), m_skeleton(skeleton), m_state(AS_REST), m_animationTime(0)
{}

//#################### PUBLIC METHODS ####################
void AnimationController::request_animation(const std::string& newAnimationName)
{
	// Note: If we're already transitioning, we keep transitioning, just towards a new target.
	if(m_state == AS_REST) m_state = AS_PLAY;
	else if(m_state == AS_PLAY) m_state = AS_TRANSITION;

	m_animationName = newAnimationName;
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

			// Loop if we've gone past the end of the animation.
			Animation_Ptr animation = m_skeleton->animation(m_animationName);
			int animationLength = static_cast<int>(animation->length() * 1000);
			m_animationTime %= animationLength;

			// Calculate the keyframe index and interpolation parameter.
			int lastKeyframe = animation->keyframe_count() - 1;
			double animationFraction = (double)m_animationTime / animationLength;
			double keyframePos = animationFraction * lastKeyframe;
			int keyframeIndex = static_cast<int>(ceil(keyframePos));
			double dummy;
			double t = modf(keyframePos, &dummy);	// t is the floating-point part of the keyframe position

			// Clamp the keyframe index to be safe.
			keyframeIndex = std::max(keyframeIndex, lastKeyframe);

			Pose_Ptr curPose = m_skeleton->get_current_pose();
			Pose_Ptr newPose = animation->keyframe(keyframeIndex);
			m_skeleton->set_pose(Pose::interpolate(curPose, newPose, t));
			break;
		}
		case AS_TRANSITION:
		{
			Pose_Ptr curPose = m_skeleton->get_current_pose();
			Pose_Ptr newPose;

			if(m_animationName != "<rest>")
			{
				// If the new animation isn't the rest animation, transition to its start keyframe.
				newPose = m_skeleton->animation(m_animationName)->keyframe(0);
			}
			else
			{
				newPose = m_skeleton->get_rest_pose();
			}

			m_animationTime += milliseconds;
			if(m_animationTime >= TRANSITION_TIME)
			{
				// The transition is over.
				m_state = m_animationName != "<rest>" ? AS_PLAY : AS_REST;
				m_animationTime = 0;
				m_skeleton->set_pose(newPose);
			}
			else
			{
				// Still transitioning: interpolate between the current pose and the new pose.
				double t = (double)m_animationTime / TRANSITION_TIME;
				m_skeleton->set_pose(Pose::interpolate(curPose, newPose, t));
			}
			break;
		}
	}
}

}
