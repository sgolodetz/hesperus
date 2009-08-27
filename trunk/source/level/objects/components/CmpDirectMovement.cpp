/***
 * hesperus: CmpDirectMovement.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpDirectMovement.h"

#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpDirectMovement::load(const Properties&)
{
	return IObjectComponent_Ptr(new CmpDirectMovement);
}

//#################### PUBLIC METHODS ####################
const std::list<OnionUtil::Transition_CPtr>& CmpDirectMovement::recent_transitions() const
{
	return m_recentTransitions;
}

std::pair<std::string,Properties> CmpDirectMovement::save() const
{
	return std::make_pair("DirectMovement", Properties());
}

void CmpDirectMovement::update_recent_transitions(const OnionUtil::Transition_CPtr& transition)
{
	assert(transition && transition->location != NULL);

	// Remove any recent transition planes which the entity's no longer on.
	const Vector3d& location = *transition->location;
	for(std::list<OnionUtil::Transition_CPtr>::iterator it=m_recentTransitions.begin(), iend=m_recentTransitions.end(); it!=iend;)
	{
		if(classify_point_against_plane(location, *(*it)->plane) == CP_COPLANAR) ++it;
		else it = m_recentTransitions.erase(it);
	}

	// Add the latest transition.
	m_recentTransitions.push_front(transition);
}

}
