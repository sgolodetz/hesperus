/***
 * hesperus: AnimationController.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ANIMATIONCONTROLLER
#define H_HESP_ANIMATIONCONTROLLER

#include "Mesh.h"
#include "Skeleton.h"

namespace hesp {

class AnimationController
{
	//#################### CONSTANTS ####################
private:
	enum
	{
		TRANSITION_TIME = 50		// the desired length of an animation transition (in ms)
	};

	enum State
	{
		AS_REST,					// not playing any animation
		AS_PLAY,					// playing an animation
		AS_TRANSITION				// transitioning between animations
	};

	//#################### PRIVATE VARIABLES ####################
private:
	Mesh_Ptr m_mesh;
	Skeleton_Ptr m_skeleton;

	State m_state;
	std::string m_animationName;	// the name of the current animation
	int m_animationTime;			// the number of ms for which the current animation (or transition) has been playing

	//#################### CONSTRUCTORS ####################
public:
	AnimationController(const Mesh_Ptr& mesh, const Skeleton_Ptr& skeleton);

	//#################### PUBLIC METHODS ####################
public:
	void request_animation(const std::string& newAnimationName);
	void update(int milliseconds);

	//#################### PRIVATE METHODS ####################
private:
	void update_skeleton(int milliseconds);
};

}

#endif
