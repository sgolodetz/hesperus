/***
 * hesperus: CmpDirectMovement.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPDIRECTMOVEMENT
#define H_HESP_CMPDIRECTMOVEMENT

#include "ICmpDirectMovement.h"

namespace hesp {

class CmpDirectMovement : public virtual ICmpDirectMovement
{
	//#################### PRIVATE VARIABLES ####################
private:
	// Records the details of recent times the entity would have crossed a wall into solid space (had we not stopped it)
	std::list<OnionUtil::Transition_Ptr> m_recentTransitions;

	//#################### PUBLIC METHODS ####################
public:
	const std::list<OnionUtil::Transition_Ptr>& recent_transitions() const;
	void update_recent_transitions(const OnionUtil::Transition_Ptr& transition);
};

}

#endif
