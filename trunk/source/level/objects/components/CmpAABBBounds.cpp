/***
 * hesperus: CmpAABBBounds.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpAABBBounds.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpAABBBounds::CmpAABBBounds(const std::vector<int>& aabbIndices, int pose)
:	m_aabbIndices(aabbIndices), m_pose(pose)
{}

//#################### STATIC FACTORY METHODS ####################
IComponent_Ptr CmpAABBBounds::load(const Properties& properties)
{
	return IComponent_Ptr(new CmpAABBBounds(properties.get<std::vector<int> >("AABBs"), properties.get<int>("Pose")));
}

//#################### PUBLIC METHODS ####################
const std::vector<int>& CmpAABBBounds::aabb_indices() const
{
	return m_aabbIndices;
}

int CmpAABBBounds::cur_aabb_index() const
{
	return m_aabbIndices[m_pose];
}

int CmpAABBBounds::pose() const
{
	return m_pose;
}
std::pair<std::string,Properties> CmpAABBBounds::save() const
{
	Properties properties;
	properties.set("AABBs", m_aabbIndices);
	properties.set("Pose", m_pose);
	return std::make_pair("AABBBounds", properties);
}

void CmpAABBBounds::set_pose(int pose)
{
	m_pose = pose;
}

}
