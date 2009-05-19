/***
 * hesperus: CmpCollision.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPCOLLISION
#define H_HESP_CMPCOLLISION

#include "ICmpCollision.h"

namespace hesp {

class CmpCollision : public ICmpCollision
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<int> m_aabbIndices;
	int m_pose;

	//#################### CONSTRUCTORS ####################
public:
	CmpCollision(const std::vector<int>& aabbIndices, int pose);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IComponent_Ptr create(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	const std::vector<int>& aabb_indices() const;
	int pose() const;
	void set_pose(int pose);
};

}

#endif
