/***
 * hesperus: CmpPosition.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpPosition.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpPosition::CmpPosition(const Vector3d& position)
:	m_position(position)
{}

//#################### STATIC FACTORY METHODS ####################
IComponent_Ptr CmpPosition::load(const Properties& properties)
{
	return IComponent_Ptr(new CmpPosition(properties.get<Vector3d>("Position")));
}

//#################### PUBLIC METHODS ####################
const Vector3d& CmpPosition::position() const
{
	return m_position;
}

std::pair<std::string,Properties> CmpPosition::save() const
{
	Properties properties;
	properties.set("Position", m_position);
	return std::make_pair("Position", properties);
}

void CmpPosition::set_position(const Vector3d& position)
{
	m_position = position;
}

}
