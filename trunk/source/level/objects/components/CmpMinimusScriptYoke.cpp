/***
 * hesperus: CmpMinimusScriptYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpMinimusScriptYoke.h"

#include <source/level/objects/components/ICmpAABBBounds.h>
#include <source/level/objects/components/ICmpMeshMovement.h>
#include <source/level/objects/components/ICmpModelRender.h>
#include <source/level/objects/components/ICmpOrientation.h>
#include <source/level/objects/components/ICmpPhysics.h>
#include <source/level/objects/components/ICmpPosition.h>
#include <source/level/objects/yokes/minimus/MinimusScriptYoke.h>
namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpMinimusScriptYoke::CmpMinimusScriptYoke(const std::string& scriptName, const ASXEngine_Ptr& engine, const bf::path& baseDir)
:	m_scriptName(scriptName), m_engine(engine), m_baseDir(baseDir)
{}

//#################### STATIC FACTORY METHODS ####################
IComponent_Ptr CmpMinimusScriptYoke::load(const Properties& properties)
{
	return IComponent_Ptr(new CmpMinimusScriptYoke(properties.get_actual<std::string>("Script"),
												   properties.get_actual<ASXEngine_Ptr>("AIEngine"),
												   properties.get_actual<bf::path>("BaseDir")));
}

//#################### PUBLIC METHODS ####################
void CmpMinimusScriptYoke::check_dependencies() const
{
	check_dependency<ICmpAABBBounds>();
	check_dependency<ICmpMeshMovement>();
	check_dependency<ICmpModelRender>();
	check_dependency<ICmpOrientation>();
	check_dependency<ICmpPhysics>();
	check_dependency<ICmpPosition>();
}

std::vector<ObjectCommand_Ptr> CmpMinimusScriptYoke::generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons,
																	   const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets)
{
	if(!m_yoke) m_yoke.reset(new MinimusScriptYoke(m_objectID, m_objectManager, m_scriptName, m_engine, m_baseDir));
	return m_yoke->generate_commands(input, polygons, tree, navDatasets);
}

std::pair<std::string,Properties> CmpMinimusScriptYoke::save() const
{
	Properties properties;
	properties.set_actual("Script", m_scriptName);
	return std::make_pair("MinimusScriptYoke", properties);
}

}
