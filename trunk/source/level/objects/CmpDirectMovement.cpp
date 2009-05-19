/***
 * hesperus: CmpDirectMovement.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpDirectMovement.h"

#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### STATIC FACTORY METHODS ####################
IComponent_Ptr CmpDirectMovement::create(const Properties&)
{
	return IComponent_Ptr(new CmpDirectMovement);
}

//#################### PUBLIC METHODS ####################
const std::list<OnionUtil::Transition_Ptr>& CmpDirectMovement::recent_transitions() const
{
	return m_recentTransitions;
}

void CmpDirectMovement::update_recent_transitions(const OnionUtil::Transition_Ptr& transition)
{
	assert(transition && transition->location != NULL);

	// Remove any recent transition planes which the entity's no longer on.
	const Vector3d& location = *transition->location;
	for(std::list<OnionUtil::Transition_Ptr>::iterator it=m_recentTransitions.begin(), iend=m_recentTransitions.end(); it!=iend;)
	{
		if(classify_point_against_plane(location, *(*it)->plane) == CP_COPLANAR) ++it;
		else it = m_recentTransitions.erase(it);
	}

	// Add the latest transition.
	m_recentTransitions.push_front(transition);
}

}
