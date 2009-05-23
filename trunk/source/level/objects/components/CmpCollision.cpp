/***
 * hesperus: CmpCollision.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpCollision.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpCollision::CmpCollision(const std::vector<int>& aabbIndices, int pose)
:	m_aabbIndices(aabbIndices), m_pose(pose)
{}

//#################### STATIC FACTORY METHODS ####################
IComponent_Ptr CmpCollision::load(const Properties& properties)
{
	return IComponent_Ptr(new CmpCollision(properties.get_actual<std::vector<int> >("AABBs"), properties.get_actual<int>("Pose")));
}

//#################### PUBLIC METHODS ####################
const std::vector<int>& CmpCollision::aabb_indices() const
{
	return m_aabbIndices;
}

int CmpCollision::pose() const
{
	return m_pose;
}
std::pair<std::string,Properties> CmpCollision::save() const
{
	Properties properties;
	properties.set_actual("AABBs", m_aabbIndices);
	properties.set_actual("Pose", m_pose);
	return std::make_pair("Collision", properties);
}

void CmpCollision::set_pose(int pose)
{
	m_pose = pose;
}

}
