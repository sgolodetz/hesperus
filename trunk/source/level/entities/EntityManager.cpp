/***
 * hesperus: EntityManager.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntityManager.h"

#include <iostream>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>
#include <source/io/EntDefFileUtil.h>
#include <source/io/LineIO.h>
#include <source/level/yokes/user/UserBipedYoke.h>
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "LookComponent.h"
#include "VariablePositionComponent.h"
#include "VisibilityComponent.h"
#include "YokeComponent.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
/**
Constructs an entity manager containing a set of entities loaded from the specified std::istream.

@param is			The std::istream
@param settingsDir	The location of the directory containing the project settings files (e.g. the entity definitions file)
@throws Exception	If EOF is encountered whilst trying to read the entities
*/
EntityManager::EntityManager(std::istream& is, const boost::filesystem::path& settingsDir)
{
	LineIO::read_checked_line(is, "Entities");
	LineIO::read_checked_line(is, "{");

	// Read in the DefinitionFile section.
	LineIO::read_checked_line(is, "DefinitionFile");
	LineIO::read_checked_line(is, "{");

		// Read in the AABBs.
		LineIO::read_line(is, m_entDefFilename, "entity definitions filename");
		m_aabbs = EntDefFileUtil::load_aabbs_only((settingsDir / m_entDefFilename).file_string());

	LineIO::read_checked_line(is, "}");

	// Read in the Instances section.
	LineIO::read_checked_line(is, "Instances");
	LineIO::read_checked_line(is, "{");

		std::string line;
		LineIO::read_line(is, line, "entity count");
		int entityCount;
		try							{ entityCount = lexical_cast<int,std::string>(line); }
		catch(bad_lexical_cast&)	{ throw Exception("The entity count was not a number"); }

		for(int i=0; i<entityCount; ++i)
		{
			load_entity(is);
		}

	LineIO::read_checked_line(is, "}");

	LineIO::read_checked_line(is, "}");
}

//#################### PUBLIC METHODS ####################
const AABB3d& EntityManager::aabb(int n) const
{
	if(n < 0 || n >= static_cast<int>(m_aabbs.size())) throw Exception("AABB index out of range");
	else return m_aabbs[n];
}

void EntityManager::save(std::ostream& os) const
{
	os << "Entities\n";
	os << "{\n";

	os << "DefinitionFile\n";
	os << "{\n";
	os << m_entDefFilename << '\n';
	os << "}\n";

	os << "Instances\n";
	os << "{\n";

	int entityCount = static_cast<int>(m_entities.size());
	os << entityCount << '\n';
	for(int i=0; i<entityCount; ++i)
	{
		m_entities[i]->save(os);
	}

	os << "}\n";

	os << "}\n";
}

Entity_Ptr EntityManager::player() const
{
	return m_player;
}

const std::vector<Entity_Ptr>& EntityManager::visibles() const
{
	return m_visibles;
}

const std::vector<Entity_Ptr>& EntityManager::yokeables() const
{
	return m_yokeables;
}

//#################### PRIVATE METHODS ####################
void EntityManager::load_entity(std::istream& is)
{
	std::string line;
	LineIO::read_line(is, line, "entity class");

	size_t i = line.find(' ');
	if(line.length() < i+2) throw Exception("Missing entity class after Instance");
	std::string entityClass = line.substr(i+1);

	LineIO::read_checked_line(is, "{");

	Entity_Ptr entity;
	ICollisionComponent_Ptr collisionComponent;
	IHealthComponent_Ptr healthComponent;
	ILookComponent_Ptr lookComponent;
	IPositionComponent_Ptr positionComponent;
	IVisibilityComponent_Ptr visibilityComponent;

	if(entityClass == "Player")
	{
		if(m_player) throw Exception("The level contains multiple Player entities");

		entity.reset(new Entity("Player"));
		m_player = entity;

		collisionComponent.reset(new CollisionComponent(is));
		healthComponent.reset(new HealthComponent(is));
		lookComponent.reset(new LookComponent(is));
		positionComponent.reset(new VariablePositionComponent(is));
		visibilityComponent.reset(new VisibilityComponent(is));

		LineIO::read_checked_line(is, "}");
	}
	else if(entityClass == "Guard")
	{
		entity.reset(new Entity("Guard"));

		collisionComponent.reset(new CollisionComponent(is));
		healthComponent.reset(new HealthComponent(is));
		lookComponent.reset(new LookComponent(is));
		positionComponent.reset(new VariablePositionComponent(is));
		visibilityComponent.reset(new VisibilityComponent(is));

		LineIO::read_checked_line(is, "}");
	}
	else
	{
		skip_entity(is, entityClass);
		return;
	}

	// Set the entity components.
	entity->set_collision_component(collisionComponent);
	entity->set_health_component(healthComponent);
	entity->set_look_component(lookComponent);
	entity->set_position_component(positionComponent);
	entity->set_visibility_component(visibilityComponent);

	// Add the newly-added entity to the relevant arrays.
	int nextEntity = static_cast<int>(m_entities.size());
	entity->set_id(nextEntity);
	m_entities.push_back(entity);
	if(entity->visibility_component()) m_visibles.push_back(entity);

	// Yoke the entity if necessary.
	IYokeComponent_Ptr yokeComponent;
	if(entityClass == "Player")
	{
		yokeComponent.reset(new YokeComponent(Yoke_Ptr(new UserBipedYoke(entity))));
	}
	else if(entityClass == "Guard")
	{
		// TODO: Connect an AI yoke.
	}

	entity->set_yoke_component(yokeComponent);
	if(entity->yoke_component()) m_yokeables.push_back(entity);
}

void EntityManager::skip_entity(std::istream& is, const std::string& entityClass)
{
	std::cout << "Unknown entity class: " << entityClass << std::endl;

	std::string line;
	while(std::getline(is, line))
	{
		if(line == "}") return;
	}

	// If we get here, it's because we didn't encounter a } before reaching the end of the file.
	throw Exception("Unexpected EOF whilst trying to skip entity " + entityClass);
}

}
