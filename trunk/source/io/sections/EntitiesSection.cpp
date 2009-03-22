/***
 * hesperus: EntitiesSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntitiesSection.h"

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/io/EntDefFileUtil.h>
#include <source/io/util/EntityComponents.h>
#include <source/io/util/FieldIO.h>
#include <source/level/entities/AnimationComponent.h>
#include <source/level/entities/CollisionComponent.h>
#include <source/level/entities/HealthComponent.h>
#include <source/level/entities/NavComponent.h>
#include <source/level/entities/PhysicsComponent.h>
#include <source/level/entities/VariableCameraComponent.h>
#include <source/level/entities/YokeComponent.h>
#include <source/level/yokes/null/NullYoke.h>
#include <source/level/yokes/user/UserBipedYoke.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Constructs an entity manager containing a set of entities loaded from the specified std::istream.

@param is			The std::istream
@param settingsDir	The location of the directory containing the project settings files (e.g. the entity definitions file)
@throws Exception	If EOF is encountered whilst trying to read the entities
*/
EntityManager_Ptr EntitiesSection::load(std::istream& is, const boost::filesystem::path& settingsDir)
{
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
		EntDefFileUtil::load((settingsDir / entDefFilename).file_string(), aabbs, entityComponentsMap);

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

			std::map<std::string,EntityComponents>::const_iterator jt = entityComponentsMap.find(entityType);
			if(jt == entityComponentsMap.end()) throw Exception("Cannot create entity instance of type: " + entityType);
			const EntityComponents& entityComponents = jt->second;

			Entity_Ptr entity(new Entity(entityType));

			if(entityComponents.has("Animation"))
			{
				IAnimationComponent_Ptr animationComponent(new AnimationComponent(is));
				entity->set_animation_component(animationComponent);
			}
			if(entityComponents.has("VariableCamera"))
			{
				ICameraComponent_Ptr cameraComponent(new VariableCameraComponent(is));
				entity->set_camera_component(cameraComponent);
			}
			if(entityComponents.has("Collision"))
			{
				ICollisionComponent_Ptr collisionComponent(new CollisionComponent(is));
				entity->set_collision_component(collisionComponent);
			}
			if(entityComponents.has("Health"))
			{
				IHealthComponent_Ptr healthComponent(new HealthComponent(is));
				entity->set_health_component(healthComponent);
			}
			if(entityComponents.has("Nav"))
			{
				entity->set_nav_component(INavComponent_Ptr(new NavComponent));
			}
			if(entityComponents.has("Physics"))
			{
				IPhysicsComponent_Ptr physicsComponent(new PhysicsComponent(is));
				entity->set_physics_component(physicsComponent);
			}
			if(entityComponents.has("Yoke"))
			{
				std::string yokeType = FieldIO::read_field(is, "Yoke");
				Yoke_Ptr yoke;

				// Note:	We should replace this with a yoke factory if the number of yokes increases.
				//			It's probably not worth the extra code for the moment.
				if(yokeType == "User")		{ yoke.reset(new UserBipedYoke(entity)); }
				else if(yokeType == "Bot")	{ yoke.reset(new NullYoke); /*MinimusGotoPositionYoke(entity, Vector3d(14,6,6)));*/ /* TEMPORARY */ }
				else if(yokeType == "None")	{ yoke.reset(new NullYoke); }
				else						{ throw Exception("Unknown yoke type: " + yokeType); }

				if(yoke)
				{
					IYokeComponent_Ptr yokeComponent(new YokeComponent(yoke, yokeType));
					entity->set_yoke_component(yokeComponent);
				}
			}

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
