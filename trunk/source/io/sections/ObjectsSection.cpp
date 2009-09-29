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

#include <source/io/util/LineIO.h>
#include <source/level/objects/base/IObjectComponent.h>
#include <source/level/objects/base/ObjectManager.h>
#include <source/level/objects/yokes/minimus/MinimusScriptYoke.h>
#include <source/util/Properties.h>

namespace hesp {

//#################### LOADING METHODS ####################
ObjectManager_Ptr ObjectsSection::load(std::istream& is, const BoundsManager_CPtr& boundsManager,
									   const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes,
									   const boost::filesystem::path& baseDir)
{
	// Set up the shared scripting engine.
	ASXEngine_Ptr aiEngine(new ASXEngine);
	MinimusScriptYoke::register_for_scripting(aiEngine);

	ObjectManager_Ptr objectManager(new ObjectManager(boundsManager, componentPropertyTypes));

	LineIO::read_checked_line(is, "Objects");
	LineIO::read_checked_line(is, "{");

	int objectCount = FieldIO::read_typed_trimmed_field<int>(is, "Count");
	for(int i=0; i<objectCount; ++i)
	{
		ObjectSpecification specification = load_object_specification(is, componentPropertyTypes, aiEngine, baseDir);
		objectManager->create_object(specification);
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
ObjectSpecification ObjectsSection::load_object_specification(std::istream& is, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes,
															  const ASXEngine_Ptr& aiEngine, const boost::filesystem::path& baseDir)
{
	typedef void (*LoadFunc)(Properties&, const std::string&, const std::string&);
	static std::map<std::string,LoadFunc> loadFuncs;
	static bool done = false;
	if(!done)
	{
		loadFuncs["double"] = load_property<double>;
		loadFuncs["int"] = load_property<int>;
		loadFuncs["ObjectID"] = load_property<ObjectID>;
		loadFuncs["string"] = load_property<std::string>;
		loadFuncs["Vector3d"] = load_property<Vector3d>;
		loadFuncs["[int]"] = load_property<std::vector<int> >;
		loadFuncs["[string]"] = load_property<std::vector<std::string> >;
		loadFuncs["{ObjectID}"] = load_property<std::set<ObjectID> >;
		loadFuncs["string -> int"] = load_property<std::map<std::string,int> >;
		loadFuncs["string -> (string -> Vector3d)"] = load_property<std::map<std::string,std::map<std::string,Vector3d> > >;
		done = true;
	}

	ObjectSpecification specification;

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
					std::map<std::string,LoadFunc>::iterator loadFunc = loadFuncs.find(type);
					if(loadFunc != loadFuncs.end()) (*(loadFunc->second))(properties, name, value);
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

		specification.add_component(componentName, properties);
	}

	return specification;
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
	typedef void (*SaveFunc)(std::ostream&, const Properties&, const std::string&);
	static std::map<std::string,SaveFunc> saveFuncs;
	static bool done = false;
	if(!done)
	{
		saveFuncs["double"] = save_property<double>;
		saveFuncs["int"] = save_property<int>;
		saveFuncs["ObjectID"] = save_property<ObjectID>;
		saveFuncs["string"] = save_property<std::string>;
		saveFuncs["Vector3d"] = save_property<Vector3d>;
		saveFuncs["[int]"] = save_property<std::vector<int> >;
		saveFuncs["[string]"] = save_property<std::vector<std::string> >;
		saveFuncs["{ObjectID}"] = save_property<std::set<ObjectID> >;
		saveFuncs["string -> int"] = save_property<std::map<std::string,int> >;
		saveFuncs["string -> (string -> Vector3d)"] = save_property<std::map<std::string,std::map<std::string,Vector3d> > >;
		done = true;
	}

	os << "\tObject\n";
	os << "\t{\n";

	for(size_t i=0, size=components.size(); i<size; ++i)
	{
		const IObjectComponent_Ptr& component = components[i];
		std::string componentName = component->own_type();
		Properties properties = component->save();

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

				std::map<std::string,SaveFunc>::iterator saveFunc = saveFuncs.find(type);
				if(saveFunc != saveFuncs.end()) (*(saveFunc->second))(os, properties, name);
				else throw Exception("The type " + type + " is not currently supported");
			}
			os << "\t\t}\n";
		}
	}

	os << "\t}\n";
}

}
