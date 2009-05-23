/***
 * hesperus: CmpCollectible.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpCollectible.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpCollectible::CmpCollectible(const std::string& scriptName)
:	m_scriptName(scriptName)
{
	// TODO
}

//#################### STATIC FACTORY METHODS ####################
IComponent_Ptr CmpCollectible::load(const Properties& properties)
{
	return IComponent_Ptr(new CmpCollectible(properties.get_actual<std::string>("Script")));
}

//#################### PUBLIC METHODS ####################
std::pair<std::string,Properties> CmpCollectible::save() const
{
	Properties properties;
	properties.set_actual("Script", m_scriptName);
	return std::make_pair("Collectible", properties);
}

}
