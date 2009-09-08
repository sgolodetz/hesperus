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
	std::list<OnionUtil::Transition_CPtr> m_recentTransitions;

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties&);

	//#################### PUBLIC METHODS ####################
public:
	const std::list<OnionUtil::Transition_CPtr>& recent_transitions() const;
	Properties save() const;
	void update_recent_transitions(const OnionUtil::Transition_CPtr& transition);
};

}

#endif
