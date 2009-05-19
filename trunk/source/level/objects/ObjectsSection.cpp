/***
 * hesperus: ObjectsSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ObjectsSection.h"

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/tuple/tuple.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;
namespace bf = boost::filesystem;

#include <source/io/util/FieldIO.h>
#include <source/util/Properties.h>
#include "CmpCollision.h"
#include "CmpDirectMovement.h"
#include "CmpMeshMovement.h"
#include "CmpMinimusScriptYoke.h"
#include "CmpOrientation.h"
#include "CmpPhysics.h"
#include "CmpPosition.h"
#include "CmpRender.h"
#include "CmpUserBipedYoke.h"
#include "YokeMinimusScript.h"

namespace hesp {

//#################### LOADING METHODS ####################
ObjectManager_Ptr ObjectsSection::load(std::istream& is, const std::vector<AABB3d>& aabbs,
									   const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes,
									   const boost::filesystem::path& baseDir)
{
	// Set up the shared scripting engine.
	ASXEngine_Ptr aiEngine(new ASXEngine);
	YokeMinimusScript::register_for_scripting(aiEngine);

	ObjectManager_Ptr objectManager(new ObjectManager(aabbs));

	LineIO::read_checked_line(is, "Objects");
	LineIO::read_checked_line(is, "{");

	int objectCount = FieldIO::read_typed_trimmed_field<int>(is, "Count");
	for(int i=0; i<objectCount; ++i)
	{
		std::vector<IComponent_Ptr> components = load_object(is, componentPropertyTypes, aiEngine, baseDir);
		objectManager->create_object(components);
	}

	LineIO::read_checked_line(is, "}");

	return objectManager;
}

//#################### LOADING SUPPORT METHODS ####################
std::vector<IComponent_Ptr> ObjectsSection::load_object(std::istream& is, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes,
														const ASXEngine_Ptr& aiEngine, const bf::path& baseDir)
{
	std::vector<IComponent_Ptr> components;

	LineIO::read_checked_trimmed_line(is, "Object");
	LineIO::read_checked_trimmed_line(is, "{");

	std::string line;
	for(;;)
	{
		LineIO::read_trimmed_line(is, line, "object component name");
		if(line == "}") break;
		if(line.length() == 0) continue;

		bool hasFields = line[line.length()-1] != ';';
		std::string componentName = hasFields ? line : line.substr(0, line.length()-1);

		Properties properties;
		if(hasFields)
		{
			LineIO::read_checked_trimmed_line(is, "{");
			for(;;)
			{
				LineIO::read_trimmed_line(is, line, "object component property");
				if(line == "}") break;

				// Parse the field.
				std::string name, value;
				boost::tie(name, value) = FieldIO::parse_field(line);

				// Lookup the type of the property.
				std::string type = lookup_property_type(componentName, name, componentPropertyTypes);

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

		// Add the AI engine and base directory as properties (they're needed by some of the components, e.g. yokes).
		properties.set_actual<ASXEngine_Ptr>("AIEngine", aiEngine);
		properties.set_actual<bf::path>("BaseDir", baseDir);

		components.push_back(invoke_component_creator(componentName, properties));
	}

	return components;
}

std::string ObjectsSection::lookup_property_type(const std::string& componentName, const std::string& propertyName,
												 const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes)
{
	std::map<std::string,std::map<std::string,std::string> >::const_iterator it = componentPropertyTypes.find(componentName);
	if(it == componentPropertyTypes.end()) throw Exception("Unknown component: " + componentName);

	std::map<std::string,std::string>::const_iterator jt = it->second.find(propertyName);
	if(jt == it->second.end()) throw Exception("Unknown property (in component " + componentName + "): " + propertyName);

	return jt->second;
}

std::vector<int> ObjectsSection::string_to_intarray(const std::string& s)
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

//#################### COMPONENT CREATOR METHODS ####################
#define ADD_CREATOR(c) componentCreators[#c] = &Cmp##c::create

std::map<std::string,ObjectsSection::ComponentCreator>& ObjectsSection::component_creators()
{
	static std::map<std::string,ComponentCreator> componentCreators;

	static bool done = false;
	if(!done)
	{
		ADD_CREATOR(Collision);
		ADD_CREATOR(DirectMovement);
		ADD_CREATOR(MeshMovement);
		ADD_CREATOR(MinimusScriptYoke);
		ADD_CREATOR(Orientation);
		ADD_CREATOR(Physics);
		ADD_CREATOR(Position);
		ADD_CREATOR(Render);
		ADD_CREATOR(UserBipedYoke);
		done = true;
	}

	return componentCreators;
}

#undef ADD_CREATOR

IComponent_Ptr ObjectsSection::invoke_component_creator(const std::string& componentName, const Properties& properties)
{
	std::map<std::string,ComponentCreator>& creators = component_creators();
	std::map<std::string,ComponentCreator>::iterator it = creators.find(componentName);
	if(it != creators.end()) return (*(it->second))(properties);
	else throw Exception("No creator registered for components of type " + componentName);
}

}
