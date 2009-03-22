/***
 * hesperus: CollisionComponent.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CollisionComponent.h"

#include <source/io/util/FieldIO.h>
#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
CollisionComponent::CollisionComponent(std::istream& is)
{
	m_aabbIndices = FieldIO::read_intarray_field(is, "AABBs");
	m_pose = FieldIO::read_typed_field<int>(is, "Pose");
}

//#################### PUBLIC METHODS ####################
const std::vector<int>& CollisionComponent::aabb_indices() const
{
	return m_aabbIndices;
}

int CollisionComponent::pose() const
{
	return m_pose;
}

const std::list<OnionTree::Transition_Ptr>& CollisionComponent::recent_transitions() const
{
	return m_recentTransitions;
}

void CollisionComponent::save(std::ostream& os) const
{
	FieldIO::write_intarray_field(os, "AABBs", m_aabbIndices);
	FieldIO::write_typed_field(os, "Pose", m_pose);
}

void CollisionComponent::set_pose(int pose)
{
	if(std::find(m_aabbIndices.begin(), m_aabbIndices.end(), pose) != m_aabbIndices.end())
	{
		m_pose = pose;
	}
}

void CollisionComponent::update_recent_transitions(const OnionTree::Transition_Ptr& transition)
{
	assert(transition->location != NULL);

	// Remove any recent transition planes which the entity's no longer on.
	const Vector3d& location = *transition->location;
	for(std::list<OnionTree::Transition_Ptr>::iterator it=m_recentTransitions.begin(), iend=m_recentTransitions.end(); it!=iend;)
	{
		if(classify_point_against_plane(location, *(*it)->plane) == CP_COPLANAR) ++it;
		else it = m_recentTransitions.erase(it);
	}

	// Add the latest transition.
	m_recentTransitions.push_front(transition);
}

}
