/***
 * hesperus: EntityManager.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntityManager.h"

#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>
#include <source/io/EntDefFileUtil.h>
#include <source/io/FieldIO.h>
#include <source/math/vectors/Vector3.h>

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

const std::vector<BipedEntity_Ptr>& EntityManager::bipeds() const
{
	return m_bipeds;
}

void EntityManager::output(std::ostream& os)
{
	os << "Entities\n";
	os << "{\n";

	os << "DefinitionFile\n";
	os << "{\n";
	os << m_entDefFilename << '\n';
	os << "}\n";

	os << "Instances\n";
	os << "{\n";

	// Calculate and output the entity count.
	int entityCount = 0;
	if(m_player) ++entityCount;
	// TODO

	os << entityCount << '\n';

	// Output the entities.
	if(m_player) m_player->save(os);
	// TODO

	os << "}\n";

	os << "}\n";
}

Player_Ptr EntityManager::player() const
{
	return m_player;
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

	if(entityClass == "Player")
	{
		if(m_player) throw Exception("The level contains multiple Player entities");
		m_player = Player::load(is);
		m_bipeds.push_back(m_player);
	}
	else skip_entity(is, entityClass);
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
