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
};

}

#endif
