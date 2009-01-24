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
#include <source/io/FileSectionUtil.h>
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
	FileSectionUtil::read_checked_line(is, "Entities");
	FileSectionUtil::read_checked_line(is, "{");

	// Read in the DefinitionFile section.
	FileSectionUtil::read_checked_line(is, "DefinitionFile");
	FileSectionUtil::read_checked_line(is, "{");

		// Read in the AABBs.
		FileSectionUtil::read_line(is, m_entDefFilename, "entity definitions filename");
		m_aabbs = EntDefFileUtil::load_aabbs_only((settingsDir / m_entDefFilename).file_string());

	FileSectionUtil::read_checked_line(is, "}");

	// Read in the Instances section.
	FileSectionUtil::read_checked_line(is, "Instances");
	FileSectionUtil::read_checked_line(is, "{");

		std::string line;
		FileSectionUtil::read_line(is, line, "entity count");
		int entityCount;
		try							{ entityCount = lexical_cast<int,std::string>(line); }
		catch(bad_lexical_cast&)	{ throw Exception("The entity count was not a number"); }

		for(int i=0; i<entityCount; ++i)
		{
			load_entity(is);
		}

	FileSectionUtil::read_checked_line(is, "}");

	FileSectionUtil::read_checked_line(is, "}");
}

//#################### PUBLIC METHODS ####################
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
	read_line(is, line, "entity class");

	size_t i = line.find(' ');
	if(line.length() < i+2) throw Exception("Missing entity class after Instance");
	std::string entityClass = line.substr(i+1);

	read_checked_line(is, "{");

	if(entityClass == "Player")
	{
		if(m_player) throw Exception("The level contains multiple Player entities");
		m_player = load_player(is);
	}
	else skip_entity(is, entityClass);
}

Player_Ptr EntityManager::load_player(std::istream& is)
{
	std::vector<int> aabbIndices = read_intarray_field(is, "AABBs");
	std::string modelFilename = read_field(is, "GameModel");
	int health = read_typed_field<int>(is, "Health");
	int pose = read_typed_field<int>(is, "Pose");
	Vector3d position = read_typed_field<Vector3d>(is, "Position");

	read_checked_line(is, "}");

	return Player_Ptr(new Player(aabbIndices, modelFilename, health, pose, position));
}

void EntityManager::read_checked_line(std::istream& is, const std::string& expected)
{
	std::string line;
	read_line(is, line, expected);
	if(line != expected) throw Exception("Expected " + expected);
}

std::string EntityManager::read_field(std::istream& is, const std::string& expectedFieldName)
{
	std::string field;

	read_line(is, field, expectedFieldName);

	size_t i = field.find('=');
	if(i == std::string::npos) throw Exception("Bad field " + expectedFieldName);

	std::string fieldName = field.substr(0, i-1);
	if(fieldName != expectedFieldName) throw Exception("Bad field name " + fieldName + ", expected " + expectedFieldName);

	if(field.length() <= i+2) throw Exception("Missing field value for " + expectedFieldName);
	field = field.substr(i+2);

	return field;
}

std::vector<int> EntityManager::read_intarray_field(std::istream& is, const std::string& expectedFieldName)
{
	std::vector<int> arr;

	std::string fieldString = read_field(is, expectedFieldName);

	typedef boost::char_separator<char> sep;
	typedef boost::tokenizer<sep> tokenizer;

	tokenizer tok(fieldString.begin(), fieldString.end(), sep("[,]"));
	std::vector<std::string> tokens(tok.begin(), tok.end());
	for(size_t i=0, size=tokens.size(); i<size; ++i)
	{
		arr.push_back(lexical_cast<int,std::string>(tokens[i]));
	}

	return arr;
}

void EntityManager::read_line(std::istream& is, std::string& line, const std::string& description)
{
	if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read " + description);
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
