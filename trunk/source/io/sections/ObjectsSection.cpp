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
#include <source/level/objects/components/CmpAABBBounds.h>
#include <source/level/objects/components/CmpDirectMovement.h>
#include <source/level/objects/components/CmpInventory.h>
#include <source/level/objects/components/CmpItemActivatable.h>
#include <source/level/objects/components/CmpMeshMovement.h>
#include <source/level/objects/components/CmpMinimusScriptYoke.h>
#include <source/level/objects/components/CmpModelRender.h>
#include <source/level/objects/components/CmpOrientation.h>
#include <source/level/objects/components/CmpOwnable.h>
#include <source/level/objects/components/CmpPhysics.h>
#include <source/level/objects/components/CmpPosition.h>
#include <source/level/objects/components/CmpUsable.h>
#include <source/level/objects/components/CmpUserBipedYoke.h>
#include <source/level/objects/yokes/minimus/MinimusScriptYoke.h>
#include <source/util/Properties.h>

namespace hesp {

//#################### LOADING METHODS ####################
ObjectManager_Ptr ObjectsSection::load(std::istream& is, const std::vector<AABB3d>& aabbs,
									   const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes,
									   const boost::filesystem::path& baseDir)
{
	// Set up the shared scripting engine.
	ASXEngine_Ptr aiEngine(new ASXEngine);
	MinimusScriptYoke::register_for_scripting(aiEngine);

	ObjectManager_Ptr objectManager(new ObjectManager(aabbs, componentPropertyTypes));

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

//#################### SAVING METHODS ####################
void ObjectsSection::save(std::ostream& os, const ObjectManager_Ptr& objectManager)
{
	objectManager->consolidate_object_ids();

	os << "Objects\n";
	os << "{\n";

	os << '\t';
	int objectCount = objectManager->object_count();
	FieldIO::write_typed_field(os, "Count", objectCount);

	// Note:	The objects with the specified IDs are guaranteed to exist because we called consolidate_object_ids() above.
	//			This would not necessarily be the case otherwise, as the IDs of the objects may not form a contiguous series.
	for(int i=0; i<objectCount; ++i)
	{
		std::vector<IComponent_Ptr> components = objectManager->get_components(ObjectID(i));
		save_object(os, components, objectManager->component_property_types());
	}

	os << "}\n";
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
				// FIXME: It's better to parse the type and call an appropriate function here.
				try
				{
					if(type == "double")				properties.set_actual(name, lexical_cast<double,std::string>(value));
					else if(type == "int")				properties.set_actual(name, lexical_cast<int,std::string>(value));
					else if(type == "string")			properties.set_actual(name, value);
					else if(type == "Vector3d")			properties.set_actual(name, lexical_cast<Vector3d,std::string>(value));
					else if(type == "[int]")			properties.set_actual(name, string_to_intarray(value));
					else if(type == "->(string,int)")	properties.set_actual(name, string_to_stringintmap(value));
					else throw Exception("The type " + type + " is not currently supported");
				}
				catch(bad_lexical_cast&)
				{
					throw Exception("The value " + value + " for " + name + " was not of the right type");
				}
			}
		}

		// Add the AI engine and base directory as properties (they're needed by some of the components, e.g. yokes).
		properties.set_actual("AIEngine", aiEngine);
		properties.set_actual("BaseDir", baseDir);

		components.push_back(invoke_component_loader(componentName, properties));
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

std::map<std::string,int> ObjectsSection::string_to_stringintmap(const std::string& s)
{
	// FIXME:	This parser only works as long as the map key strings don't contain the characters "[", "]", "," or ";".
	//			It needs to be redone properly in the next refactoring.

	std::map<std::string,int> ret;

	typedef boost::char_separator<char> sep;
	typedef boost::tokenizer<sep> tokenizer;

	tokenizer outerTok(s.begin(), s.end(), sep("[;]"));
	std::vector<std::string> outerTokens(outerTok.begin(), outerTok.end());

	for(size_t i=0, size=outerTokens.size(); i<size; ++i)
	{
		tokenizer innerTok(outerTokens[i].begin(), outerTokens[i].end(), sep("(,)"));
		std::vector<std::string> innerTokens(innerTok.begin(), innerTok.end());
		if(innerTokens.size() != 2) throw bad_lexical_cast();
		std::string key = innerTokens[0];
		int value = lexical_cast<int,std::string>(innerTokens[1]);
		ret[key] = value;
	}

	return ret;
}

//#################### SAVING SUPPORT METHODS ####################
std::string ObjectsSection::intarray_to_string(const std::vector<int>& arr)
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

void ObjectsSection::save_object(std::ostream& os, const std::vector<IComponent_Ptr>& components,
								 const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes)
{
	os << "\tObject\n";
	os << "\t{\n";

	for(size_t i=0, size=components.size(); i<size; ++i)
	{
		const IComponent_Ptr& component = components[i];
		std::string componentName;
		Properties properties;
		boost::tie(componentName, properties) = component->save();

		os << "\t\t" << componentName;

		std::map<std::string,std::map<std::string,std::string> >::const_iterator jt = componentPropertyTypes.find(componentName);
		if(jt == componentPropertyTypes.end()) throw Exception("Unknown component: " + componentName);
		const std::map<std::string,std::string>& types = jt->second;

		if(types.empty())
		{
			os << ";\n";
		}
		else
		{
			os << "\n\t\t{\n";
			for(std::map<std::string,std::string>::const_iterator kt=types.begin(), kend=types.end(); kt!=kend; ++kt)
			{
				const std::string& name = kt->first;
				const std::string& type = kt->second;

				os << "\t\t\t";

				// FIXME: It's better to parse the type and call an appropriate function here.
				if(type == "double")				FieldIO::write_typed_field(os, name, properties.get_actual<double>(name));
				else if(type == "int")				FieldIO::write_typed_field(os, name, properties.get_actual<int>(name));
				else if(type == "string")			FieldIO::write_typed_field(os, name, properties.get_actual<std::string>(name));
				else if(type == "Vector3d")			FieldIO::write_typed_field(os, name, properties.get_actual<Vector3d>(name));
				else if(type == "[int]")			FieldIO::write_typed_field(os, name, intarray_to_string(properties.get_actual<std::vector<int> >(name)));
				else if(type == "->(string,int)")	FieldIO::write_typed_field(os, name, stringintmap_to_string(properties.get_actual<std::map<std::string,int> >(name)));
				else throw Exception("The type " + type + " is not currently supported");
			}
			os << "\t\t}\n";
		}
	}

	os << "\t}\n";
}

std::string ObjectsSection::stringintmap_to_string(const std::map<std::string,int>& m)
{
	std::ostringstream os;

	os << '[';
	for(std::map<std::string,int>::const_iterator it=m.begin(), iend=m.end(); it!=iend;)
	{
		os << '(' << it->first << ',' << it->second << ')';
		++it;
		if(it != iend) os << ';';
	}
	os << ']';

	return os.str();
}

//#################### COMPONENT CREATOR METHODS ####################
#define ADD_LOADER(c) componentLoaders[#c] = &Cmp##c::load

std::map<std::string,ObjectsSection::ComponentLoader>& ObjectsSection::component_loaders()
{
	static std::map<std::string,ComponentLoader> componentLoaders;

	static bool done = false;
	if(!done)
	{
		ADD_LOADER(AABBBounds);
		ADD_LOADER(DirectMovement);
		ADD_LOADER(Inventory);
		ADD_LOADER(ItemActivatable);
		ADD_LOADER(MeshMovement);
		ADD_LOADER(MinimusScriptYoke);
		ADD_LOADER(ModelRender);
		ADD_LOADER(Orientation);
		ADD_LOADER(Ownable);
		ADD_LOADER(Physics);
		ADD_LOADER(Position);
		ADD_LOADER(Usable);
		ADD_LOADER(UserBipedYoke);
		done = true;
	}

	return componentLoaders;
}

#undef ADD_LOADER

IComponent_Ptr ObjectsSection::invoke_component_loader(const std::string& componentName, const Properties& properties)
{
	std::map<std::string,ComponentLoader>& creators = component_loaders();
	std::map<std::string,ComponentLoader>::iterator it = creators.find(componentName);
	if(it != creators.end()) return (*(it->second))(properties);
	else throw Exception("No loader registered for components of type " + componentName);
}

}
