/***
 * hesperus: CollisionComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_COLLISIONCOMPONENT
#define H_HESP_COLLISIONCOMPONENT

#include <source/io/FieldIO.h>
#include <source/math/geom/GeomUtil.h>
#include "ICollisionComponent.h"

namespace hesp {

class CollisionComponent : public ICollisionComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<int> m_aabbIndices;
	int m_pose;
	std::list<OnionTree::Transition_Ptr> m_recentTransitions;	// records the details of recent times the entity would have crossed a wall into solid space (had we not stopped it)

	//#################### CONSTRUCTORS ####################
public:
	CollisionComponent(std::istream& is)
	{
		m_aabbIndices = FieldIO::read_intarray_field(is, "AABBs");
		m_pose = FieldIO::read_typed_field<int>(is, "Pose");
	}

	//#################### PUBLIC METHODS ####################
public:
	const std::vector<int>& aabb_indices() const
	{
		return m_aabbIndices;
	}

	int pose() const
	{
		return m_pose;
	}

	const std::list<OnionTree::Transition_Ptr>& recent_transitions() const
	{
		return m_recentTransitions;
	}

	void save(std::ostream& os) const
	{
		FieldIO::write_intarray_field(os, "AABBs", m_aabbIndices);
		FieldIO::write_typed_field(os, "Pose", m_pose);
	}

	void set_pose(int pose)
	{
		if(std::find(m_aabbIndices.begin(), m_aabbIndices.end(), pose) != m_aabbIndices.end())
		{
			m_pose = pose;
		}
	}

	void update_recent_transitions(const OnionTree::Transition_Ptr& transition)
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
};

}

#endif
