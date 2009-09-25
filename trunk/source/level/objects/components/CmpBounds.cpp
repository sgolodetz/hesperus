/***
 * hesperus: CmpBounds.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpBounds.h"

#include <source/util/Properties.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpBounds::CmpBounds(const std::string& boundsGroup, const std::string& posture)
:	m_boundsGroup(boundsGroup), m_posture(posture)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpBounds::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpBounds(properties.get<std::string>("Group"), properties.get<std::string>("Posture")));
}

//#################### PUBLIC METHODS ####################
const std::string& CmpBounds::bounds_group() const
{
	return m_boundsGroup;
}

const std::string& CmpBounds::posture() const
{
	return m_posture;
}

Properties CmpBounds::save() const
{
	Properties properties;
	properties.set("Group", m_boundsGroup);
	properties.set("Posture", m_posture);
	return properties;
}

void CmpBounds::set_posture(const std::string& posture)
{
	m_posture = posture;
}

}
