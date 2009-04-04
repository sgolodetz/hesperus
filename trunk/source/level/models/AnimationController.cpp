/***
 * hesperus: AnimationController.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AnimationController.h"

#include <cmath>

namespace {

//#################### CONSTANTS ####################
enum
{
	TRANSITION_TIME = 50		// the desired length of an animation transition (in ms)
};

}

namespace hesp {

//#################### CONSTRUCTORS ####################
AnimationController::AnimationController(bool interpolateKeyframes)
:	m_interpolateKeyframes(interpolateKeyframes), m_state(AS_REST), m_animationTime(0)
{}

//#################### PUBLIC METHODS ####################
const Pose_Ptr& AnimationController::get_pose() const
{
	return m_pose;
}

void AnimationController::request_animation(const std::string& newAnimationName)
{
	// If we're already playing this animation, ignore the request.
	if(m_animationName == newAnimationName) return;

	// Note: If we're already transitioning, we keep transitioning, just towards a new target.
	if(m_state == AS_REST)
	{
		m_state = AS_TRANSITION;
		m_transitionStart = m_skeleton->get_rest_pose();
	}
	else if(m_state == AS_PLAY)
	{
		m_state = AS_TRANSITION;
		m_transitionStart = get_pose();
	}

	m_animationName = newAnimationName;
	m_animationTime = 0;
}

void AnimationController::set_skeleton(const Skeleton_Ptr& skeleton)
{
	m_skeleton = skeleton;
}

void AnimationController::update(int milliseconds)
{
	update_pose(milliseconds);
}

//#################### PRIVATE METHODS ####################
void AnimationController::set_pose(const Pose_Ptr& pose)
{
	m_pose = pose;
}

void AnimationController::update_pose(int milliseconds)
{
	switch(m_state)
	{
		case AS_REST:
		{
			set_pose(m_skeleton->get_rest_pose());
			break;
		}
		case AS_PLAY:
		{
			Animation_Ptr animation = m_skeleton->animation(m_animationName);
			int animationLength = static_cast<int>(animation->length() * 1000);

			m_animationTime += milliseconds;

			// Loop if we've gone past the end of the animation.
			m_animationTime %= animationLength;

			// Calculate the keyframe index and interpolation parameter.
			int lastKeyframe = animation->keyframe_count() - 1;
			double animationFraction = (double)m_animationTime / animationLength;
			double keyframePos = animationFraction * lastKeyframe;
			int keyframeIndex = static_cast<int>(ceil(keyframePos));
			double dummy;
			double t = modf(keyframePos, &dummy);	// t is the floating-point part of the keyframe position

			// Clamp the keyframe index to be safe.
			keyframeIndex = std::min(keyframeIndex, lastKeyframe);

			Pose_Ptr newPose = animation->keyframe(keyframeIndex);
			if(m_interpolateKeyframes)
			{
				int oldKeyframeIndex = (keyframeIndex + lastKeyframe) % animation->keyframe_count();
				Pose_Ptr oldPose = animation->keyframe(oldKeyframeIndex);
				set_pose(Pose::interpolate(oldPose, newPose, t));
			}
			else
			{
				set_pose(newPose);
			}
			break;
		}
		case AS_TRANSITION:
		{
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
				m_transitionStart.reset();
				set_pose(newPose);
			}
			else
			{
				// Still transitioning: interpolate between the transition start pose and the new pose.
				double t = (double)m_animationTime / TRANSITION_TIME;
				set_pose(Pose::interpolate(m_transitionStart, newPose, t));
			}
			break;
		}
	}
}

}
