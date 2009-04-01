/***
 * hesperus: MinimusScriptYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MINIMUSSCRIPTYOKE
#define H_HESP_MINIMUSSCRIPTYOKE

#include <boost/filesystem/operations.hpp>

#include <ASXEngine.h>
#include <ASXRefType.h>

#include <source/level/entities/Entity.h>

namespace hesp {

/**
This class represents a script-controlled yoke for the Minimus bot.
*/
class MinimusScriptYoke : public Yoke, public ASXRefType<MinimusScriptYoke>
{
	//#################### PRIVATE VARIABLES ####################
private:
	ASXEngine_Ptr m_engine;
	ASXModule_Ptr m_module;

	Entity_Ptr m_biped;
	Yoke_Ptr m_subyoke;

	//#################### CONSTRUCTORS ####################
public:
	MinimusScriptYoke(const Entity_Ptr& biped, const ASXEngine_Ptr& engine, const boost::filesystem::path& baseDir);

	//#################### PUBLIC METHODS ####################
public:
	void add_ref();
	std::vector<EntityCommand_Ptr> generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets);
	static void register_for_scripting(const ASXEngine_Ptr& engine);
	void release();
	static std::string type_string();

	//#################### SCRIPT INTERFACE ####################
private:
	void clear_subyoke();
	void goto_position(double x, double y, double z);
	bool subyoke_active() const;
	bool subyoke_exists() const;
};

}

#endif
