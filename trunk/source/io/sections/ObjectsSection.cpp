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
#include <source/io/util/PropFormatter.h>
#include <source/io/util/PropReader.h>
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
		std::vector<IObjectComponent_Ptr> components = load_object(is, componentPropertyTypes, aiEngine, baseDir);
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
		std::vector<IObjectComponent_Ptr> components = objectManager->get_components(ObjectID(i));
		save_object(os, components, objectManager->component_property_types());
	}

	os << "}\n";
}

//#################### LOADING SUPPORT METHODS ####################
std::vector<IObjectComponent_Ptr> ObjectsSection::load_object(std::istream& is, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes,
														const ASXEngine_Ptr& aiEngine, const bf::path& baseDir)
{
	std::vector<IObjectComponent_Ptr> components;

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
				// FIXME: Replace the long if block with a lookup.
				try
				{
					if(type == "double")				properties.set(name, read_property<double>(value));
					else if(type == "int")				properties.set(name, read_property<int>(value));
					else if(type == "string")			properties.set(name, read_property<std::string>(value));
					else if(type == "Vector3d")			properties.set(name, read_property<Vector3d>(value));
					else if(type == "[int]")			properties.set(name, read_property<std::vector<int> >(value));
					else if(type == "string -> int")	properties.set(name, read_property<std::map<std::string,int> >(value));
					else throw Exception("The type " + type + " is not currently supported");
				}
				catch(bad_lexical_cast&)
				{
					throw Exception("The value " + value + " for " + name + " was not of the right type");
				}
			}
		}

		// Add the AI engine and base directory as properties (they're needed by some of the components, e.g. yokes).
		properties.set("AIEngine", aiEngine);
		properties.set("BaseDir", baseDir);

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

//#################### SAVING SUPPORT METHODS ####################
void ObjectsSection::save_object(std::ostream& os, const std::vector<IObjectComponent_Ptr>& components,
								 const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes)
{
	os << "\tObject\n";
	os << "\t{\n";

	for(size_t i=0, size=components.size(); i<size; ++i)
	{
		const IObjectComponent_Ptr& component = components[i];
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

				// FIXME: Replace the long if block with a lookup.
				if(type == "double")				FieldIO::write_typed_field(os, name, format_property(properties.get<double>(name)));
				else if(type == "int")				FieldIO::write_typed_field(os, name, format_property(properties.get<int>(name)));
				else if(type == "string")			FieldIO::write_typed_field(os, name, format_property(properties.get<std::string>(name)));
				else if(type == "Vector3d")			FieldIO::write_typed_field(os, name, format_property(properties.get<Vector3d>(name)));
				else if(type == "[int]")			FieldIO::write_typed_field(os, name, format_property(properties.get<std::vector<int> >(name)));
				else if(type == "string -> int")	FieldIO::write_typed_field(os, name, format_property(properties.get<std::map<std::string,int> >(name)));
				else throw Exception("The type " + type + " is not currently supported");
			}
			os << "\t\t}\n";
		}
	}

	os << "\t}\n";
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

IObjectComponent_Ptr ObjectsSection::invoke_component_loader(const std::string& componentName, const Properties& properties)
{
	std::map<std::string,ComponentLoader>& loaders = component_loaders();
	std::map<std::string,ComponentLoader>::iterator it = loaders.find(componentName);
	if(it != loaders.end()) return (*(it->second))(properties);
	else throw Exception("No loader registered for components of type " + componentName);
}

}
