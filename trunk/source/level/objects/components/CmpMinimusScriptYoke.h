/***
 * hesperus: CmpMinimusScriptYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPMINIMUSSCRIPTYOKE
#define H_HESP_CMPMINIMUSSCRIPTYOKE

#include <boost/filesystem/operations.hpp>

#include <ASXEngine.h>

#include "ICmpYoke.h"

namespace hesp {

class CmpMinimusScriptYoke : public ICmpYoke
{
	//#################### PRIVATE VARIABLES ####################
private:
	IYoke_Ptr m_yoke;

	std::string m_scriptName;
	ASXEngine_Ptr m_engine;
	boost::filesystem::path m_baseDir;

	//#################### CONSTRUCTORS ####################
public:
	CmpMinimusScriptYoke(const std::string& scriptName, const ASXEngine_Ptr& engine, const boost::filesystem::path& baseDir);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	std::vector<ObjectCommand_Ptr> generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets);
	std::pair<std::string,Properties> save() const;
};

}

#endif