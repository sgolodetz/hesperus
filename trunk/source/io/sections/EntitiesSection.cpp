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
			LineIO::read_line(is, line, "entity type");
			size_t n = line.find(' ');
			if(n+2 > line.length()) throw Exception("Missing entity type after Instance");
			std::string entityType = line.substr(n+1);

			LineIO::read_checked_line(is, "{");

			// FIXME: Do this properly (requires changing the file format).
			Entity::Properties properties(entityType);
			properties.characterModel.reset(new std::string(FieldIO::read_typed_field<std::string>(is, "GameModel")));
			properties.position.reset(new Vector3d(FieldIO::read_typed_field<Vector3d>(is, "Position")));
			properties.look.reset(new Vector3d(FieldIO::read_typed_field<Vector3d>(is, "Look")));
			properties.aabbIndices.reset(new std::vector<int>(FieldIO::read_intarray_field(is, "AABBs")));
			properties.pose.reset(new int(FieldIO::read_typed_field<int>(is, "Pose")));
			properties.health.reset(new int(FieldIO::read_typed_field<int>(is, "Health")));
			properties.mass.reset(new double(FieldIO::read_typed_field<double>(is, "Mass")));
			properties.velocity.reset(new Vector3d);

			Entity_Ptr entity(new Entity(properties));

			std::string yokeType = FieldIO::read_field(is, "Yoke");
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

			if(yoke) entity->set_yoke(yoke, yokeType);

			entities.push_back(entity);

			LineIO::read_checked_line(is, "}");
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
		entities[i]->save(os);
	}

	os << "}\n";

	os << "}\n";
}

}
