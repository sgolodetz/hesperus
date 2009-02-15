/***
 * hesperus: CollisionComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_COLLISIONCOMPONENT
#define H_HESP_COLLISIONCOMPONENT

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
	CollisionComponent(std::istream& is);

	//#################### PUBLIC METHODS ####################
public:
	const std::vector<int>& aabb_indices() const;
	int pose() const;
	const std::list<OnionTree::Transition_Ptr>& recent_transitions() const;
	void save(std::ostream& os) const;
	void set_pose(int pose);
	void update_recent_transitions(const OnionTree::Transition_Ptr& transition);
};

}

#endif
