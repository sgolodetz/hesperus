/***
 * hesperus: YokeMinimusScript.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "YokeMinimusScript.h"

#include <iostream>

#include <source/io/util/DirectoryFinder.h>
#include "YokeMinimusGotoPosition.h"
namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
YokeMinimusScript::YokeMinimusScript(const ObjectID& objectID, ObjectManager *objectManager, const std::string& scriptName, const ASXEngine_Ptr& engine,
									 const bf::path& baseDir)
:	m_objectID(objectID), m_objectManager(objectManager), m_engine(engine), m_initialised(false)
{
	bf::path scriptsDir = determine_scripts_directory(baseDir);
	std::string scriptFilename = (scriptsDir / (scriptName + ".as")).file_string();

	m_engine->clear_messages();
	if(!m_engine->load_and_build_script(scriptFilename, scriptName))
	{
		m_engine->output_messages(std::cout);
		throw Exception("Could not build script module " + scriptName);
	}

	m_module = m_engine->get_module(scriptName);
}

//#################### PUBLIC METHODS ####################
void YokeMinimusScript::add_ref()
{
	++m_refCount;
}

std::vector<ObjectCommand_Ptr> YokeMinimusScript::generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons,
																	const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets)
{
	if(!m_initialised)
	{
		// Run the script init method.
		ASXFunction<void(YokeMinimusScript*)> init = m_module->get_global_function("init", init);
		init(this);

		m_initialised = true;
	}

	// Run the script process method.
	ASXFunction<void(YokeMinimusScript*)> process = m_module->get_global_function("process", process);
	process(this);

	if(m_subyoke && m_subyoke->state() == YOKE_ACTIVE)
	{
		return m_subyoke->generate_commands(input, polygons, tree, navDatasets);
	}
	else return std::vector<ObjectCommand_Ptr>();
}

void YokeMinimusScript::register_for_scripting(const ASXEngine_Ptr& engine)
{
	engine->register_uninstantiable_ref_type<YokeMinimusScript>();
	engine->register_object_method(&clear_subyoke, "clear_subyoke");
	engine->register_object_method(&goto_position, "goto_position");
	engine->register_object_method(&request_animation, "request_animation");
	engine->register_object_method(&subyoke_active, "subyoke_active");
	engine->register_object_method(&subyoke_exists, "subyoke_exists");
}

void YokeMinimusScript::release()
{
	if(--m_refCount == 0) delete this;
}

std::string YokeMinimusScript::type_string()
{
	// Note: This is the name that will be used to refer to YokeMinimusScript within scripts.
	return "ScriptYoke";
}

//#################### SCRIPT INTERFACE ####################
void YokeMinimusScript::clear_subyoke()
{
	m_subyoke.reset();
}

void YokeMinimusScript::goto_position(double x, double y, double z)
{
	m_subyoke.reset(new YokeMinimusGotoPosition(m_objectID, m_objectManager, Vector3d(x,y,z)));
}

void YokeMinimusScript::request_animation(const std::string& name)
{
	// NYI
	throw 23;
}

bool YokeMinimusScript::subyoke_active() const
{
	return subyoke_exists() && m_subyoke->state() == YOKE_ACTIVE;
}

bool YokeMinimusScript::subyoke_exists() const
{
	return m_subyoke != NULL;
}

}
