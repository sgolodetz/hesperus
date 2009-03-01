/***
 * hesperus: FileSectionUtil_Load.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FileSectionUtil.h"

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/images/BitmapLoader.h>
#include <source/io/FieldIO.h>
#include <source/level/entities/AnimationComponent.h>
#include <source/level/entities/CollisionComponent.h>
#include <source/level/entities/HealthComponent.h>
#include <source/level/entities/NavComponent.h>
#include <source/level/entities/PhysicsComponent.h>
#include <source/level/entities/VariableCameraComponent.h>
#include <source/level/entities/YokeComponent.h>
#include <source/level/yokes/minimus/MinimusGotoPositionYoke.h>
#include <source/level/yokes/null/NullYoke.h>
#include <source/level/yokes/user/UserBipedYoke.h>
#include "EntDefFileUtil.h"
#include "NavLinkFactory.h"

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads an array of AABBs from the specified std::istream.

@param is			The std::istream
@return				The AABBs
@throws Exception	If EOF is encountered whilst trying to read the AABBs
*/
std::vector<AABB3d> FileSectionUtil::load_aabbs_section(std::istream& is)
{
	std::vector<AABB3d> ret;

	LineIO::read_checked_line(is, "AABBs");
	LineIO::read_checked_line(is, "{");

	std::string line;
	while(std::getline(is, line))
	{
		if(line == "}") return ret;

		std::string::size_type i = line.find('=');
		if(i == std::string::npos) throw Exception("Bad AABB data encountered: missing equals");
		if(line.length() <= i+2) throw Exception("Missing AABB data after equals");
		line = line.substr(i+2);
		ret.push_back(read_aabb<Vector3d>(line));
	}

	// If we get here, we must have run out of lines to read before we saw a }.
	throw Exception("Unexpected EOF whilst trying to read the AABBs");
}

/**
Loads an EntityComponents section from a std::istream. These are present in entity definition files.

@param is	The std::istream
*/
void FileSectionUtil::load_entity_components_section(std::istream& is)
{
	// Note: This function skips over the EntityComponents section because it's only relevant to the map editor.

	LineIO::read_checked_line(is, "EntityComponents");
	LineIO::read_checked_line(is, "{");

	std::string line;
	int bracketCount = 1;
	while(bracketCount > 0)
	{
		LineIO::read_line(is, line, "entity components section");
		if(line == "{") ++bracketCount;
		if(line == "}") --bracketCount;
	}
}

/**
Loads an EntityTypes section from a std::istream. These are present in entity definition files.
*/
std::map<std::string,EntityComponents> FileSectionUtil::load_entity_types_section(std::istream& is)
{
	// Note:	This function only loads in which components are contained in each entity type.
	//			The rest of the information is only relevant to the map editor.

	std::map<std::string,EntityComponents> entityComponentsMap;

	LineIO::read_checked_line(is, "EntityTypes");
	LineIO::read_checked_line(is, "{");

	std::string line;
	for(;;)
	{
		LineIO::read_line(is, line, "entity type");
		if(line == "}") break;

		typedef boost::char_separator<char> sep;
		typedef boost::tokenizer<sep> tokenizer;
		tokenizer tok(line.begin(), line.end(), sep(":, "));
		std::vector<std::string> tokens(tok.begin(), tok.end());

		std::string type = tokens[0];
		EntityComponents components;

		int tokenCount = static_cast<int>(tokens.size());
		for(int i=1; i<tokenCount; ++i)
		{
			components.set(tokens[i], true);
		}

		entityComponentsMap.insert(std::make_pair(type, components));

		// Skip the rest of the entity type information.
		LineIO::read_checked_line(is, "{");
		do
		{
			LineIO::read_line(is, line, "entity type");
		} while(line != "}");
	}

	return entityComponentsMap;
}

/**
Constructs an entity manager containing a set of entities loaded from the specified std::istream.

@param is			The std::istream
@param settingsDir	The location of the directory containing the project settings files (e.g. the entity definitions file)
@throws Exception	If EOF is encountered whilst trying to read the entities
*/
EntityManager_Ptr FileSectionUtil::load_entities_section(std::istream& is, const boost::filesystem::path& settingsDir)
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

/**
Loads a lightmap prefix from the specified std::istream.

@param is			The std::istream
@return				The lightmap prefix
@throws Exception	If EOF is encountered whilst trying to read the lightmap prefix
*/
std::string FileSectionUtil::load_lightmap_prefix_section(std::istream& is)
{
	std::string lightmapPrefix;

	LineIO::read_checked_line(is, "LightmapPrefix");
	LineIO::read_checked_line(is, "{");
	LineIO::read_line(is, lightmapPrefix, "lightmap prefix");
	LineIO::read_checked_line(is, "}");

	return lightmapPrefix;
}

/**
Loads an array of lightmaps from the specified std::istream.

@param is			The std::istream
@return				The lightmaps
@throws Exception	If EOF is encountered whilst trying to read the lightmaps
*/
std::vector<Image24_Ptr> FileSectionUtil::load_lightmaps_section(std::istream& is)
{
	std::vector<Image24_Ptr> lightmaps;

	LineIO::read_checked_line(is, "Lightmaps");
	LineIO::read_checked_line(is, "{");

	std::string line;
	LineIO::read_line(is, line, "lightmap count");
	int lightmapCount;
	try							{ lightmapCount = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The lightmap count was not an integer"); }

	lightmaps.resize(lightmapCount);
	for(int i=0; i<lightmapCount; ++i)
	{
		lightmaps[i] = BitmapLoader::load_image24(is);
	}

	if(is.get() != '\n') throw Exception("Expected newline after lightmaps");

	LineIO::read_checked_line(is, "}");

	return lightmaps;
}

/**
Loads an array of lights from the specified std::istream.

@param is	The std::istream
@return		The array of lights
*/
std::vector<Light> FileSectionUtil::load_lights_section(std::istream& is)
{
	std::vector<Light> lights;

	std::string line;

	LineIO::read_checked_line(is, "Lights");
	LineIO::read_checked_line(is, "{");

	// Read in the light count.
	int lightCount;
	try
	{
		std::getline(is, line);
		lightCount = lexical_cast<int,std::string>(line);
		lights.reserve(lightCount);
	}
	catch(bad_lexical_cast&) { throw Exception("The light count was not an integer"); }

	// Read in the lights, one per line.
	for(int i=0; i<lightCount; ++i)
	{
		if(!std::getline(is, line))
		{
			std::ostringstream oss;
			oss << "Unexpected EOF at line " << i << " in the lights file";
			throw Exception(oss.str());
		}

		// Parse the line.
		typedef boost::char_separator<char> sep;
		typedef boost::tokenizer<sep> tokenizer;
		tokenizer tok(line.begin(), line.end(), sep(" "));
		std::vector<std::string> tokens(tok.begin(), tok.end());

		if(tokens.size() != 11) throw Exception("Bad light data at line " + lexical_cast<std::string,int>(i));

		std::vector<std::string> positionComponents(&tokens[1], &tokens[4]);
		std::vector<std::string> colourComponents(&tokens[6], &tokens[9]);

		Vector3d position(positionComponents);
		Colour3d colour(colourComponents);
		double falloffRadius;
		try							{ falloffRadius = lexical_cast<double,std::string>(tokens[10]); }
		catch(bad_lexical_cast&)	{ throw Exception("The falloff radius was not a number at line " + lexical_cast<std::string,int>(i)); }
		lights.push_back(Light(position, colour, falloffRadius));
	}

	LineIO::read_checked_line(is, "}");

	return lights;
}

/**
Loads an array of navigation datasets from the specified std::istream.

@param is	The std::istream
@return		The navigation datasets
*/
std::vector<NavDataset_Ptr> FileSectionUtil::load_nav_section(std::istream& is)
{
	std::vector<NavDataset_Ptr> datasets;

	LineIO::read_checked_line(is, "Nav");
	LineIO::read_checked_line(is, "{");

	std::string line;
	LineIO::read_line(is, line, "navigation dataset count");
	int datasetCount;
	try							{ datasetCount = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The navigation dataset count was not an integer"); }

	for(int i=0; i<datasetCount; ++i)
	{
		LineIO::read_checked_line(is, "Dataset");
		LineIO::read_checked_line(is, "{");

		NavMesh_Ptr navMesh = read_navmesh(is);
		AdjacencyList_Ptr adjList = read_adjacency_list(is);
		PathTable_Ptr pathTable = read_path_table(is);

		datasets.push_back(NavDataset_Ptr(new NavDataset(adjList, navMesh, pathTable)));

		LineIO::read_checked_line(is, "}");
	}

	LineIO::read_checked_line(is, "}");

	return datasets;
}

/**
Loads an onion tree from the specified std::istream.

@param is	The std::istream
@return		The onion tree
*/
OnionTree_Ptr FileSectionUtil::load_onion_tree_section(std::istream& is)
{
	LineIO::read_checked_line(is, "OnionTree");
	LineIO::read_checked_line(is, "{");
	OnionTree_Ptr tree = OnionTree::load_postorder_text(is);
	LineIO::read_checked_line(is, "}");
	return tree;
}

/**
Loads a BSP tree from the specified std::istream.

@param is	The std::istream
@return		The BSP tree
*/
BSPTree_Ptr FileSectionUtil::load_tree_section(std::istream& is)
{
	std::string line;
	LineIO::read_checked_line(is, "BSPTree");
	LineIO::read_checked_line(is, "{");
	BSPTree_Ptr tree = BSPTree::load_postorder_text(is);
	LineIO::read_checked_line(is, "}");
	return tree;
}

/**
Loads a leaf visibility table from the specified std::istream.

@param is	The std::istream
@return		The visibility table
*/
LeafVisTable_Ptr FileSectionUtil::load_vis_section(std::istream& is)
{
	LeafVisTable_Ptr leafVis;

	std::string line;

	LineIO::read_checked_line(is, "VisTable");
	LineIO::read_checked_line(is, "{");

	// Read in the size of the vis table.
	LineIO::read_line(is, line, "vis table size");
	int size;
	try							{ size = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The vis table size was not an integer"); }

	// Construct an empty vis table of the right size.
	leafVis.reset(new LeafVisTable(size));

	// Read in the vis table itself.
	for(int i=0; i<size; ++i)
	{
		LineIO::read_line(is, line, "vis table row " + lexical_cast<std::string,int>(i));
		if(line.length() != size) throw Exception("Bad vis table row " + lexical_cast<std::string,int>(i));

		for(int j=0; j<size; ++j)
		{
			if(line[j] == '0') (*leafVis)(i,j) = LEAFVIS_NO;
			else if(line[j] == '1') (*leafVis)(i,j) = LEAFVIS_YES;
			else throw Exception("Bad vis table value in row " + lexical_cast<std::string,int>(i));
		}
	}

	LineIO::read_checked_line(is, "}");

	return leafVis;
}

//#################### LOADING SUPPORT METHODS ####################
/**
Reads an adjacency list from the specified std::istream.
*/
AdjacencyList_Ptr FileSectionUtil::read_adjacency_list(std::istream& is)
{
	LineIO::read_checked_line(is, "AdjacencyList");
	LineIO::read_checked_line(is, "{");

	std::string line;
	LineIO::read_line(is, line, "adjacency list size");
	int size;
	try							{ size = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The adjacency list size was not an integer"); }

	AdjacencyList_Ptr adjList(new AdjacencyList(size));

	for(int i=0; i<size; ++i)
	{
		LineIO::read_line(is, line, "adjacency list entry");

		typedef boost::char_separator<char> sep;
		typedef boost::tokenizer<sep> tokenizer;
		tokenizer tok(line.begin(), line.end(), sep(" "));
		std::vector<std::string> tokens(tok.begin(), tok.end());

		// Check the adjacency list entry for validity.
		int tokenCount = static_cast<int>(tokens.size());
		if(tokenCount == 0) throw Exception("Missing adjacency list entry " + lexical_cast<std::string,int>(i));
		if(tokenCount % 4 != 1) throw Exception("Bad adjacency list entry " + lexical_cast<std::string,int>(i));

		try
		{
			int checkI = lexical_cast<int,std::string>(tokens[0]);
			if(checkI != i) throw Exception("Bad adjacency list entry " + lexical_cast<std::string,int>(i));
		}
		catch(bad_lexical_cast&)
		{
			throw Exception("The first token in the adjacency list entry was not an integer");
		}

		// Parse the edges.
		for(int j=1; j<tokenCount; j += 4)
		{
			if(tokens[j] != "(") throw Exception("Expected ( to start adjacency list entry edge");
			if(tokens[j+3] != ")") throw Exception("Expected ) to finish adjacency list entry edge");

			try
			{
				int toNode = lexical_cast<int,std::string>(tokens[j+1]);
				float length = lexical_cast<float,std::string>(tokens[j+2]);
				adjList->add_edge(i, AdjacencyList::Edge(toNode, length));
			}
			catch(bad_lexical_cast&)
			{
				throw Exception("Bad edge in adjacency list entry " + lexical_cast<std::string,int>(i));
			}
		}
	}

	LineIO::read_checked_line(is, "}");

	return adjList;
}

/**
Reads a navigation mesh from the specified std::istream.
*/
NavMesh_Ptr FileSectionUtil::read_navmesh(std::istream& is)
{
	LineIO::read_checked_line(is, "Mesh");
	LineIO::read_checked_line(is, "{");

	// Read in the nav links.
	LineIO::read_checked_line(is, "Links");
	LineIO::read_checked_line(is, "{");

	NavLinkFactory navLinkFactory;

	std::vector<NavLink_Ptr> navLinks;

	std::string line;
	for(;;)
	{
		std::getline(is, line);
		if(line == "}") break;
		else navLinks.push_back(navLinkFactory.construct_navlink(line));
	}

	// Read in the nav polygons.
	LineIO::read_checked_line(is, "Polygons");
	LineIO::read_checked_line(is, "{");

	std::vector<NavPolygon_Ptr> navPolygons;

	for(;;)
	{
		std::getline(is, line);
		if(line == "}") break;

		std::stringstream ss;
		ss << line;

		ss >> std::skipws;

		int index, colIndex;
		std::string dummy;
		ss >> index >> colIndex >> dummy;

		if(dummy != "[") throw Exception("Expected [ to start in links in nav polygon");

		NavPolygon_Ptr poly(new NavPolygon(colIndex));

		// Read the in links.
		for(;;)
		{
			ss >> dummy;
			if(dummy == "]") break;

			try
			{
				int inLink = lexical_cast<int,std::string>(dummy);
				poly->add_in_link(inLink);
			}
			catch(bad_lexical_cast&)
			{
				throw Exception("Bad in link for nav polygon " + lexical_cast<std::string,int>(index));
			}
		}

		// Read the out links.
		ss >> dummy;
		if(dummy != "[") throw Exception("Expected [ to start out links in nav polygon");

		for(;;)
		{
			ss >> dummy;
			if(dummy == "]") break;

			try
			{
				int outLink = lexical_cast<int,std::string>(dummy);
				poly->add_out_link(outLink);
			}
			catch(bad_lexical_cast&)
			{
				throw Exception("Bad out link for nav polygon " + lexical_cast<std::string,int>(index));
			}
		}

		navPolygons.push_back(poly);
	}

	LineIO::read_checked_line(is, "}");

	return NavMesh_Ptr(new NavMesh(navPolygons, navLinks));
}

/**
Reads a (binary format) path table from the specified std::istream.
*/
PathTable_Ptr FileSectionUtil::read_path_table(std::istream& is)
{
	LineIO::read_checked_line(is, "PathTable");
	LineIO::read_checked_line(is, "{");

	std::string line;
	LineIO::read_line(is, line, "path table size");
	int size;
	try							{ size = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The path table size was not an integer"); }

	PathTable_Ptr pathTable(new PathTable(size));

	for(int i=0; i<size; ++i)
		for(int j=0; j<size; ++j)
		{
			// TODO: There may be endian issues with this if we ever port to another platform.
			is.read(reinterpret_cast<char*>(&pathTable->next_node(i,j)), sizeof(int));
			is.read(reinterpret_cast<char*>(&pathTable->cost(i,j)), sizeof(float));
		}

	if(is.get() != '\n') throw Exception("Expected newline after path table");

	LineIO::read_checked_line(is, "}");

	return pathTable;
}

}
