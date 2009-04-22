/***
 * hesperus: EntitiesSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntitiesSection.h"

#include <boost/lexical_cast.hpp>
namespace bf = boost::filesystem;
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/io/DefinitionsFile.h>
#include <source/io/util/DirectoryFinder.h>
#include <source/io/util/EntityComponents.h>
#include <source/io/util/FieldIO.h>
#include <source/level/yokes/minimus/MinimusScriptYoke.h>
#include <source/level/yokes/null/NullYoke.h>
#include <source/level/yokes/user/UserBipedYoke.h>
#include "DefinitionsSpecifierSection.h"

namespace hesp {

//#################### LOADING METHODS ####################
/**
Constructs an entity manager containing a set of entities loaded from the specified std::istream.

@param is			The std::istream
@param baseDir		The location of the project base directory
@throws Exception	If EOF is encountered whilst trying to read the entities
*/
EntityManager_Ptr EntitiesSection::load(std::istream& is, const bf::path& baseDir)
{
	// Set up the shared scripting engines.
	ASXEngine_Ptr aiEngine(new ASXEngine);
	MinimusScriptYoke::register_for_scripting(aiEngine);

	// Read in the DefinitionsSpecifier section.
	std::string definitionsFilename = DefinitionsSpecifierSection::load(is);

	// Read in the definitions file and extract the AABBs.
	bf::path settingsDir = determine_settings_directory(baseDir);
	std::vector<AABB3d> aabbs = DefinitionsFile::load_aabbs_only((settingsDir / definitionsFilename).file_string());

	// Read in the Entities section.
	LineIO::read_checked_line(is, "Entities");
	LineIO::read_checked_line(is, "{");

	// Read in the Instances section.
	LineIO::read_checked_line(is, "Instances");
	LineIO::read_checked_line(is, "{");

		std::string line;
		LineIO::read_line(is, line, "entity count");
		int entityCount;
		try							{ entityCount = lexical_cast<int,std::string>(line); }
		catch(bad_lexical_cast&)	{ throw Exception("The entity count was not a number"); }

		std::vector<Entity_Ptr> entities;
		for(int i=0; i<entityCount; ++i)
		{
			Entity_Ptr entity = load_entity(is, aiEngine, baseDir);
			entities.push_back(entity);
		}

	LineIO::read_checked_line(is, "}");		// end Instances
	LineIO::read_checked_line(is, "}");		// end Entities

	return EntityManager_Ptr(new EntityManager(entities, aabbs, definitionsFilename));
}

//#################### SAVING METHODS ####################
/**
Saves a set of entities to the specified std::ostream.

@param os				The std::ostream
@param entityManager	An EntityManager containing the entities to save
*/
void EntitiesSection::save(std::ostream& os, const EntityManager_Ptr& entityManager)
{
	DefinitionsSpecifierSection::save(os, entityManager->definitions_filename());

	os << "Entities\n";
	os << "{\n";

	os << "Instances\n";
	os << "{\n";

	const std::vector<Entity_Ptr>& entities = entityManager->entities();
	int entityCount = static_cast<int>(entities.size());
	os << entityCount << '\n';
	for(int i=0; i<entityCount; ++i)
	{
		save_entity(os, entities[i]);
	}

	os << "}\n";

	os << "}\n";
}

//#################### LOADING SUPPORT METHODS ####################
Entity_Ptr EntitiesSection::load_entity(std::istream& is, const ASXEngine_Ptr& aiEngine, const boost::filesystem::path& baseDir)
{
	// Read in the entity instance.
	Properties properties;
	LineIO::read_checked_line(is, "Instance");
	load_entity_properties(is, properties);

	// Construct it and set the appropriate yoke.
	Entity_Ptr entity(new Entity(properties));

	const std::string& yokeType = *properties.get<std::string>("Yoke");
	Yoke_Ptr yoke;

	std::string yokeClass, yokeParams;
	std::string::size_type sp = yokeType.find(' ');
	if(sp != std::string::npos)
	{
		yokeClass = yokeType.substr(0,sp);
		yokeParams = yokeType.substr(sp+1);
	}
	else yokeClass = yokeType;

	// Note:	We should replace this with a yoke factory if the number of yokes increases.
	//			It's probably not worth the extra code for the moment.
	if(yokeClass == "User")			{ yoke.reset(new UserBipedYoke(entity)); }
	else if(yokeClass == "Minimus")	{ yoke.reset(new MinimusScriptYoke(entity, yokeParams, aiEngine, baseDir)); }
	else if(yokeClass == "None")	{ yoke.reset(new NullYoke); }
	else							{ throw Exception("Unknown yoke class: " + yokeClass); }

	entity->set_yoke(yoke, yokeType);

	return entity;
}

void EntitiesSection::load_entity_properties(std::istream& is, Properties& properties)
{
	LineIO::read_checked_line(is, "{");

	// FIXME: Do this properly.
	properties.set_actual("AABBs", FieldIO::read_intarray_field(is, "AABBs"));
	properties.set_actual("Archetype", FieldIO::read_field(is, "Archetype"));
	properties.set_actual("GameModel", FieldIO::read_field(is, "GameModel"));
	properties.set_actual("Health", FieldIO::read_typed_field<int>(is, "Health"));
	properties.set_actual("Look", FieldIO::read_typed_field<Vector3d>(is, "Look"));
	properties.set_actual("Mass", FieldIO::read_typed_field<double>(is, "Mass"));
	properties.set_actual("Pose", FieldIO::read_typed_field<int>(is, "Pose"));
	properties.set_actual("Position", FieldIO::read_typed_field<Vector3d>(is, "Position"));
	properties.set_actual("Yoke", FieldIO::read_field(is, "Yoke"));

	LineIO::read_checked_line(is, "}");
}

//#################### SAVING SUPPORT METHODS ####################
void EntitiesSection::save_entity(std::ostream& os, const Entity_Ptr& entity)
{
	const Properties& properties = entity->properties();

	os << "Instance\n";
	os << "{\n";

	// FIXME: Do this properly.
	if(properties.has("AABBs")) FieldIO::write_intarray_field(os, "AABBs", *properties.get<std::vector<int> >("AABBs"));
	if(properties.has("Archetype")) FieldIO::write_typed_field(os, "Archetype", *properties.get<std::string>("Archetype"));
	if(properties.has("GameModel")) FieldIO::write_typed_field(os, "GameModel", *properties.get<std::string>("GameModel"));
	if(properties.has("Health")) FieldIO::write_typed_field(os, "Health", *properties.get<int>("Health"));
	if(properties.has("Look")) FieldIO::write_typed_field(os, "Look", *properties.get<Vector3d>("Look"));
	if(properties.has("Mass")) FieldIO::write_typed_field(os, "Mass", *properties.get<double>("Mass"));
	if(properties.has("Pose")) FieldIO::write_typed_field(os, "Pose", *properties.get<int>("Pose"));
	if(properties.has("Position")) FieldIO::write_typed_field(os, "Position", *properties.get<Vector3d>("Position"));
	if(properties.has("Yoke")) FieldIO::write_typed_field(os, "Yoke", *properties.get<std::string>("Yoke"));

	os << "}\n";
}

}
