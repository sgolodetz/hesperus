/***
 * hesperus: CmpMinimusScriptYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpMinimusScriptYoke.h"

#include "YokeMinimusScript.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpMinimusScriptYoke::CmpMinimusScriptYoke(const std::string& scriptName, const ASXEngine_Ptr& engine, const boost::filesystem::path& baseDir)
:	m_scriptName(scriptName), m_engine(engine), m_baseDir(baseDir)
{}

//#################### PUBLIC METHODS ####################
std::vector<ObjectCommand_Ptr> CmpMinimusScriptYoke::generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons,
																	   const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets)
{
	if(!m_yoke) m_yoke.reset(new YokeMinimusScript(m_objectID, m_objectManager, m_scriptName, m_engine, m_baseDir));
	return m_yoke->generate_commands(input, polygons, tree, navDatasets);
}

}
