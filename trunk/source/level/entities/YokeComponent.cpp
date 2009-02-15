/***
 * hesperus: YokeComponent.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "YokeComponent.h"

#include <source/io/FieldIO.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
YokeComponent::YokeComponent(const Yoke_Ptr& yoke, const std::string& yokeType)
:	m_yoke(yoke), m_yokeType(yokeType)
{}

//#################### PUBLIC METHODS ####################
void YokeComponent::save(std::ostream& os) const
{
	FieldIO::write_typed_field(os, "Yoke", m_yokeType);
}

void YokeComponent::set_yoke(const Yoke_Ptr& yoke, const std::string& yokeType)
{
	m_yoke = yoke;
	m_yokeType = yokeType;
}

const Yoke_Ptr& YokeComponent::yoke() const
{
	return m_yoke;
}

const std::string& YokeComponent::yoke_type() const
{
	return m_yokeType;
}

}
