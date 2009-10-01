/***
 * hesperus: CmpMinimusScriptYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPMINIMUSSCRIPTYOKE
#define H_HESP_CMPMINIMUSSCRIPTYOKE

#include <boost/filesystem/operations.hpp>

#include "ICmpYoke.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class IYoke> IYoke_Ptr;

class CmpMinimusScriptYoke : public ICmpYoke
{
	//#################### PRIVATE VARIABLES ####################
private:
	IYoke_Ptr m_yoke;

	std::string m_scriptName;
	boost::filesystem::path m_baseDir;

	//#################### CONSTRUCTORS ####################
public:
	CmpMinimusScriptYoke(const std::string& scriptName, const boost::filesystem::path& baseDir);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	void check_dependencies() const;
	std::vector<ObjectCommand_Ptr> generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const std::vector<NavDataset_Ptr>& navDatasets);
	Properties save() const;

	std::string own_type() const			{ return "MinimusScriptYoke"; }
	static std::string static_own_type()	{ return "MinimusScriptYoke"; }
};

}

#endif
