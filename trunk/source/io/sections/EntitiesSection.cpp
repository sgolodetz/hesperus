/***
 * hesperus: EntitiesSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntitiesSection.h"

#include <boost/lexical_cast.hpp>
namespace bf = boost::filesystem;
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/io/EntDefFile.h>
#include <source/io/util/DirectoryFinder.h>
#include <source/io/util/EntityComponents.h>
#include <source/io/util/FieldIO.h>
#include <source/level/yokes/minimus/MinimusScriptYoke.h>
#include <source/level/yokes/null/NullYoke.h>
#include <source/level/yokes/user/UserBipedYoke.h>

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

	// Read in the Entities section.
	LineIO::read_checked_line(is, "Entities");
	LineIO::read_checked_line(is, "{");

	// Read in the DefinitionFile section.
	LineIO::read_checked_line(is, "DefinitionFile");
	LineIO::read_checked_line(is, "{");

		// Read in the entity definition file and extract the AABBs and entity types.
		std::string entDefFilename;
		LineIO::read_line(is, entDefFilename, "entity definitions filename");
		std::vector<AABB3d> aabbs;
		std::map<std::string,EntityComponents> entityComponentsMap;
		bf::path settingsDir = determine_settings_directory(baseDir);
		EntDefFile::load((settingsDir / entDefFilename).file_string(), aabbs, entityComponentsMap);

	LineIO::read_checked_line(is, "}");

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

	LineIO::read_checked_line(is, "}");

	LineIO::read_checked_line(is, "}");

	return EntityManager_Ptr(new EntityManager(entities, aabbs, entDefFilename));
}

//#################### SAVING METHODS ####################
/**
Saves a set of entities to the specified std::ostream.

@param os				The std::ostream
@param entityManager	An EntityManager containing the entities to save
*/
void EntitiesSection::save(std::ostream& os, const EntityManager_Ptr& entityManager)
{
	os << "Entities\n";
	os << "{\n";

	os << "DefinitionFile\n";
	os << "{\n";
	os << entityManager->definition_filename() << '\n';
	os << "}\n";

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
	Properties properties;

	std::string line;
	LineIO::read_line(is, line, "entity type");
	size_t n = line.find(' ');
	if(n+2 > line.length()) throw Exception("Missing entity type after Instance");
	properties.set("EntityType", line.substr(n+1));

	LineIO::read_checked_line(is, "{");

	// FIXME: Do this properly (requires changing the file format).
	properties.set("GameModel", FieldIO::read_field(is, "GameModel"));
	properties.set("Position", FieldIO::read_typed_field<Vector3d>(is, "Position"));
	properties.set("Look", FieldIO::read_typed_field<Vector3d>(is, "Look"));
	properties.set("AABBs", FieldIO::read_intarray_field(is, "AABBs"));
	properties.set("Pose", FieldIO::read_typed_field<int>(is, "Pose"));
	properties.set("Health", FieldIO::read_typed_field<int>(is, "Health"));
	properties.set("Mass", FieldIO::read_typed_field<double>(is, "Mass"));
	properties.set("Yoke", FieldIO::read_field(is, "Yoke"));

	LineIO::read_checked_line(is, "}");

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

//#################### SAVING SUPPORT METHODS ####################
void EntitiesSection::save_entity(std::ostream& os, const Entity_Ptr& entity)
{
	const Properties& properties = entity->properties();

	os << "Instance " << *properties.get<std::string>("EntityType") << '\n';
	os << "{\n";

	// FIXME: Do this properly (requires changing the file format).
	if(properties.has("GameModel")) FieldIO::write_typed_field(os, "GameModel", *properties.get<std::string>("GameModel"));
	if(properties.has("Position")) FieldIO::write_typed_field(os, "Position", *properties.get<Vector3d>("Position"));
	if(properties.has("Look")) FieldIO::write_typed_field(os, "Look", *properties.get<Vector3d>("Look"));
	if(properties.has("AABBs")) FieldIO::write_intarray_field(os, "AABBs", *properties.get<std::vector<int> >("AABBs"));
	if(properties.has("Pose")) FieldIO::write_typed_field(os, "Pose", *properties.get<int>("Pose"));
	if(properties.has("Health")) FieldIO::write_typed_field(os, "Health", *properties.get<int>("Health"));
	if(properties.has("Mass")) FieldIO::write_typed_field(os, "Mass", *properties.get<double>("Mass"));
	if(properties.has("Yoke")) FieldIO::write_typed_field(os, "Yoke", *properties.get<std::string>("Yoke"));

	os << "}\n";
}

}
