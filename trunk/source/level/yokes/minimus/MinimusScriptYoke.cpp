/***
 * hesperus: MinimusScriptYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MinimusScriptYoke.h"

#include <iostream>

#include <source/io/util/DirectoryFinder.h>
#include "MinimusGotoPositionYoke.h"
namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
MinimusScriptYoke::MinimusScriptYoke(const Entity_Ptr& biped, const std::string& scriptName, const ASXEngine_Ptr& engine,
									 const bf::path& baseDir)
:	m_biped(biped), m_engine(engine), m_initialised(false)
{
	if(!m_biped->animation_component() ||
	   !m_biped->camera_component() ||
	   !m_biped->collision_component() ||
	   !m_biped->nav_component() ||
	   !m_biped->physics_component())
	{
		throw Exception("Couldn't attach a biped yoke to a non-biped");
	}

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
void MinimusScriptYoke::add_ref()
{
	++m_refCount;
}

std::vector<EntityCommand_Ptr> MinimusScriptYoke::generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets)
{
	if(!m_initialised)
	{
		// Run the script init method.
		ASXFunction<void(MinimusScriptYoke*)> init = m_module->get_global_function("init", init);
		init(this);

		m_initialised = true;
	}

	// Run the script process method.
	ASXFunction<void(MinimusScriptYoke*)> process = m_module->get_global_function("process", process);
	process(this);

	if(m_subyoke && m_subyoke->state() == YOKE_ACTIVE)
	{
		return m_subyoke->generate_commands(input, polygons, tree, navDatasets);
	}
	else return std::vector<EntityCommand_Ptr>();
}

void MinimusScriptYoke::register_for_scripting(const ASXEngine_Ptr& engine)
{
	engine->register_uninstantiable_ref_type<MinimusScriptYoke>();
	engine->register_object_method(&clear_subyoke, "clear_subyoke");
	engine->register_object_method(&goto_position, "goto_position");
	engine->register_object_method(&request_animation, "request_animation");
	engine->register_object_method(&subyoke_active, "subyoke_active");
	engine->register_object_method(&subyoke_exists, "subyoke_exists");
}

void MinimusScriptYoke::release()
{
	if(--m_refCount == 0) delete this;
}

std::string MinimusScriptYoke::type_string()
{
	// Note: This is the name that will be used to refer to MinimusScriptYoke within scripts.
	return "ScriptYoke";
}

//#################### SCRIPT INTERFACE ####################
void MinimusScriptYoke::clear_subyoke()
{
	m_subyoke.reset();
}

void MinimusScriptYoke::goto_position(double x, double y, double z)
{
	m_subyoke.reset(new MinimusGotoPositionYoke(m_biped, Vector3d(x,y,z)));
}

void MinimusScriptYoke::request_animation(const std::string& name)
{
	const IAnimationComponent_Ptr& animComponent = m_biped->animation_component();
	const AnimationController_Ptr& animController = animComponent->anim_controller();
	animController->request_animation(name);
}

bool MinimusScriptYoke::subyoke_active() const
{
	return subyoke_exists() && m_subyoke->state() == YOKE_ACTIVE;
}

bool MinimusScriptYoke::subyoke_exists() const
{
	return m_subyoke.get() != NULL;
}

}