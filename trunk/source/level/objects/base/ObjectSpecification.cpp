/***
 * hesperus: ObjectSpecification.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ObjectSpecification.h"

#include <source/level/objects/components/CmpBasicModelRender.h>
#include <source/level/objects/components/CmpBipedAnimChooser.h>
#include <source/level/objects/components/CmpCharacterModelRender.h>
#include <source/level/objects/components/CmpConsumeActivatable.h>
#include <source/level/objects/components/CmpHealth.h>
#include <source/level/objects/components/CmpInventory.h>
#include <source/level/objects/components/CmpItemActivatable.h>
#include <source/level/objects/components/CmpMinimusScriptYoke.h>
#include <source/level/objects/components/CmpMovement.h>
#include <source/level/objects/components/CmpOrientation.h>
#include <source/level/objects/components/CmpOwnable.h>
#include <source/level/objects/components/CmpPosition.h>
#include <source/level/objects/components/CmpProjectileWeaponUsable.h>
#include <source/level/objects/components/CmpScriptedUsable.h>
#include <source/level/objects/components/CmpSimulation.h>
#include <source/level/objects/components/CmpUserBipedYoke.h>
#include <source/util/Properties.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
void ObjectSpecification::add_component(const std::string& componentName, const Properties& properties)
{
	bool succeeded = m_components.insert(std::make_pair(componentName, properties)).second;
	if(!succeeded) throw Exception("Existing component with same name: " + componentName);
}

std::vector<IObjectComponent_Ptr> ObjectSpecification::instantiate_components() const
{
	std::vector<IObjectComponent_Ptr> ret;
	ret.reserve(m_components.size());

	for(ComponentMap::const_iterator it=m_components.begin(), iend=m_components.end(); it!=iend; ++it)
	{
		ret.push_back(invoke_component_loader(it->first, it->second));
	}

	return ret;
}

//#################### COMPONENT LOADER METHODS ####################
#define ADD_LOADER(c) componentLoaders[#c] = &Cmp##c::load

std::map<std::string,ObjectSpecification::ComponentLoader>& ObjectSpecification::component_loaders()
{
	static std::map<std::string,ComponentLoader> componentLoaders;

	static bool done = false;
	if(!done)
	{
		ADD_LOADER(BasicModelRender);
		ADD_LOADER(BipedAnimChooser);
		ADD_LOADER(CharacterModelRender);
		ADD_LOADER(ConsumeActivatable);
		ADD_LOADER(Health);
		ADD_LOADER(Inventory);
		ADD_LOADER(ItemActivatable);
		ADD_LOADER(MinimusScriptYoke);
		ADD_LOADER(Movement);
		ADD_LOADER(Orientation);
		ADD_LOADER(Ownable);
		ADD_LOADER(Position);
		ADD_LOADER(ProjectileWeaponUsable);
		ADD_LOADER(ScriptedUsable);
		ADD_LOADER(Simulation);
		ADD_LOADER(UserBipedYoke);
		done = true;
	}

	return componentLoaders;
}

#undef ADD_LOADER

IObjectComponent_Ptr ObjectSpecification::invoke_component_loader(const std::string& componentName, const Properties& properties)
{
	std::map<std::string,ComponentLoader>& loaders = component_loaders();
	std::map<std::string,ComponentLoader>::iterator it = loaders.find(componentName);
	if(it != loaders.end()) return (*(it->second))(properties);
	else throw Exception("No loader registered for components of type " + componentName);
}

}
