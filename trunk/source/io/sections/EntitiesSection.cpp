/***
 * hesperus: EntitiesSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntitiesSection.h"

#include <sstream>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/tuple/tuple.hpp>
namespace bf = boost::filesystem;
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/io/util/DirectoryFinder.h>
#include <source/io/util/FieldIO.h>
#include <source/level/yokes/minimus/MinimusScriptYoke.h>
#include <source/level/yokes/null/NullYoke.h>
#include <source/level/yokes/user/UserBipedYoke.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Constructs an entity manager containing a set of entities loaded from the specified std::istream.

@param is				The std::istream
@param aabbs			The entity AABBs
@param propertyTypes	The types of the possible entity properties
@param baseDir			The location of the project base directory
@throws Exception		If EOF is encountered whilst trying to read the entities
*/
EntityManager_Ptr EntitiesSection::load(std::istream& is, const std::vector<AABB3d>& aabbs, const std::map<std::string,std::string>& propertyTypes, const bf::path& baseDir)
{
	// Set up the shared scripting engines.
	ASXEngine_Ptr aiEngine(new ASXEngine);
	MinimusScriptYoke::register_for_scripting(aiEngine);

	// Read in the Entities section.
	LineIO::read_checked_line(is, "Entities");
	LineIO::read_checked_line(is, "{");

		std::string line;
		LineIO::read_line(is, line, "entity count");
		int entityCount;
		try							{ entityCount = lexical_cast<int,std::string>(line); }
		catch(bad_lexical_cast&)	{ throw Exception("The entity count was not a number"); }

		std::vector<Entity_Ptr> entities;
		for(int i=0; i<entityCount; ++i)
		{
			Entity_Ptr entity = load_entity(is, aiEngine, propertyTypes, baseDir);
			entities.push_back(entity);
		}

	LineIO::read_checked_line(is, "}");		// end Entities

	return EntityManager_Ptr(new EntityManager(entities, aabbs, propertyTypes));
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

	const std::map<std::string,std::string>& propertyTypes = entityManager->property_types();
	const std::vector<Entity_Ptr>& entities = entityManager->entities();
	int entityCount = static_cast<int>(entities.size());
	os << entityCount << '\n';
	for(int i=0; i<entityCount; ++i)
	{
		save_entity(os, entities[i], propertyTypes);
	}

	os << "}\n";
}

//#################### LOADING SUPPORT METHODS ####################
std::string EntitiesSection::intarray_to_string(const std::vector<int>& arr)
{
	std::ostringstream os;

	os << '[';
	for(size_t i=0, size=arr.size(); i<size; ++i)
	{
		os << arr[i];
		if(i < size-1) os << ',';
	}
	os << ']';

	return os.str();
}

Entity_Ptr EntitiesSection::load_entity(std::istream& is, const ASXEngine_Ptr& aiEngine, const std::map<std::string,std::string>& propertyTypes,
										const boost::filesystem::path& baseDir)
{
	// Read in the entity instance.
	Properties properties;
	LineIO::read_checked_line(is, "Entity");
	load_entity_properties(is, properties, propertyTypes);

	// Construct it and set the appropriate yoke.
	Entity_Ptr entity(new Entity(properties));

	const std::string& yokeType = properties.get_actual<std::string>("Yoke");
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

void EntitiesSection::load_entity_properties(std::istream& is, Properties& properties, const std::map<std::string,std::string>& propertyTypes)
{
	LineIO::read_checked_line(is, "{");

	std::string line;
	for(;;)
	{
		LineIO::read_line(is, line, "entity property");
		if(line == "}") break;

		// Parse the field.
		std::string name, value;
		boost::tie(name, value) = FieldIO::parse_field(line);

		// Lookup the type of the value.
		std::map<std::string,std::string>::const_iterator it = propertyTypes.find(name);
		if(it == propertyTypes.end()) throw Exception("Unknown property: " + name);
		const std::string& type = it->second;

		// Convert the value to the correct type and add it to the properties map.
		try
		{
			if(type == "double")			properties.set_actual(name, lexical_cast<double,std::string>(value));
			else if(type == "int")			properties.set_actual(name, lexical_cast<int,std::string>(value));
			else if(type == "string")		properties.set_actual(name, value);
			else if(type == "Vector3d")		properties.set_actual(name, lexical_cast<Vector3d,std::string>(value));
			else if(type == "[int]")		properties.set_actual(name, string_to_intarray(value));
			else throw Exception("The type " + type + " is not currently supported");
		}
		catch(bad_lexical_cast&)
		{
			throw Exception("The value " + value + " for " + name + " was not of the right type");
		}
	}
}

//#################### SAVING SUPPORT METHODS ####################
void EntitiesSection::save_entity(std::ostream& os, const Entity_Ptr& entity, const std::map<std::string,std::string>& propertyTypes)
{
	const Properties& properties = entity->properties();

	os << "Entity\n";
	os << "{\n";

	for(std::map<std::string,std::string>::const_iterator it=propertyTypes.begin(), iend=propertyTypes.end(); it!=iend; ++it)
	{
		std::string name, type;
		boost::tie(name, type) = *it;

		if(!properties.has(name)) continue;

		if(type == "double")		FieldIO::write_typed_field(os, name, properties.get_actual<double>(name));
		else if(type == "int")		FieldIO::write_typed_field(os, name, properties.get_actual<int>(name));
		else if(type == "string")	FieldIO::write_typed_field(os, name, properties.get_actual<std::string>(name));
		else if(type == "Vector3d")	FieldIO::write_typed_field(os, name, properties.get_actual<Vector3d>(name));
		else if(type == "[int]")	FieldIO::write_typed_field(os, name, intarray_to_string(properties.get_actual<std::vector<int> >(name)));
		else throw Exception("The type " + type + " is not currently supported");
	}

	os << "}\n";
}

std::vector<int> EntitiesSection::string_to_intarray(const std::string& s)
{
	typedef boost::char_separator<char> sep;
	typedef boost::tokenizer<sep> tokenizer;

	tokenizer tok(s.begin(), s.end(), sep("[,]"));
	std::vector<std::string> tokens(tok.begin(), tok.end());

	std::vector<int> arr;
	for(size_t i=0, size=tokens.size(); i<size; ++i)
	{
		arr.push_back(lexical_cast<int,std::string>(tokens[i]));
	}

	return arr;
}

}
