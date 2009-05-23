/***
 * hesperus: CmpCollectible.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpCollectible.h"

#include <source/level/objects/base/ObjectManager.h>
#include "ICmpCollision.h"

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
void CmpCollectible::check_dependencies() const
{
	check_dependency<ICmpCollision>();	// need a bounding box against which to do picking
}

void CmpCollectible::collected_by(const ObjectID& collector)
{
	// TODO
}

std::pair<std::string,Properties> CmpCollectible::save() const
{
	Properties properties;
	properties.set_actual("Script", m_scriptName);
	return std::make_pair("Collectible", properties);
}

}
