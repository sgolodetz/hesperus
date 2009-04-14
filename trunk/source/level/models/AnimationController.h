/***
 * hesperus: AnimationController.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ANIMATIONCONTROLLER
#define H_HESP_ANIMATIONCONTROLLER

#include "Skeleton.h"

namespace hesp {

class AnimationController
{
	//#################### ENUMERATIONS ####################
private:
	enum State
	{
		AS_REST,					// not playing any animation
		AS_PLAY,					// playing an animation
		AS_TRANSITION				// transitioning between animations
	};

	//#################### PRIVATE VARIABLES ####################
private:
	Skeleton_CPtr m_skeleton;
	bool m_interpolateKeyframes;

	State m_state;
	std::string m_animationName;	// the name of the current animation
	int m_animationTime;			// the number of ms for which the current animation (or transition) has been playing
	Pose_Ptr m_pose;				// the current pose
	Pose_Ptr m_transitionStart;		// the pose at the start of the transition

	//#################### CONSTRUCTORS ####################
public:
	AnimationController(bool interpolateKeyframes = false);

	//#################### PUBLIC METHODS ####################
public:
	const Pose_Ptr& get_pose() const;
	void request_animation(const std::string& newAnimationName);
	void set_skeleton(const Skeleton_Ptr& skeleton);
	void update(int milliseconds);

	//#################### PRIVATE METHODS ####################
private:
	void reset_controller();
	void set_pose(const Pose_Ptr& pose);
	void update_pose(int milliseconds);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<AnimationController> AnimationController_Ptr;
typedef shared_ptr<const AnimationController> AnimationController_CPtr;

}

#endif
