/***
 * hesperus: MinimusScriptYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MINIMUSSCRIPTYOKE
#define H_HESP_MINIMUSSCRIPTYOKE

#include <boost/filesystem/operations.hpp>

#include <ASXEngine.h>
#include <ASXRefType.h>

#include <source/level/objects/base/IYoke.h>

namespace hesp {

class MinimusScriptYoke : public IYoke, public ASXRefType<MinimusScriptYoke>
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;
	ObjectManager *m_objectManager;

	ASXEngine_Ptr m_engine;
	ASXModule_Ptr m_module;
	bool m_initialised;

	IYoke_Ptr m_subyoke;

	//#################### CONSTRUCTORS ####################
public:
	MinimusScriptYoke(const ObjectID& objectID, ObjectManager *objectManager, const std::string& scriptName, const ASXEngine_Ptr& engine, const boost::filesystem::path& baseDir);

	//#################### PUBLIC METHODS ####################
public:
	void add_ref();
	std::vector<ObjectCommand_Ptr> generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets);
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
