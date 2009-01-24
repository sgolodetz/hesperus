/***
 * hesperus: FileSectionUtil_Load.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FileSectionUtil.h"

#include <boost/lexical_cast.hpp>
namespace bf = boost::filesystem;
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/images/BitmapLoader.h>
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

	read_checked_line(is, "AABBs");
	read_checked_line(is, "{");

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
Loads a set of entities from the specified std::istream.

@param is			The std::istream
@param settingsDir	The location of the directory containing the project settings files (e.g. the entity definitions file)
@return				An EntityManager containing the loaded entities
@throws Exception	If EOF is encountered whilst trying to read the entities
*/
EntityManager_Ptr FileSectionUtil::load_entities_section(std::istream& is, const bf::path& settingsDir)
{
	read_checked_line(is, "Entities");
	read_checked_line(is, "{");

	// Read in the DefinitionFile section.
	read_checked_line(is, "DefinitionFile");
	read_checked_line(is, "{");

		// Read in the AABBs.
		std::string entDefFilename;
		read_line(is, entDefFilename, "entity definitions filename");
		std::vector<AABB3d> aabbs = EntDefFileUtil::load_aabbs_only((settingsDir / entDefFilename).file_string());

	read_checked_line(is, "}");

	EntityManager_Ptr entityManager(new EntityManager(aabbs));

	// Read in the Instances section.
	read_checked_line(is, "Instances");
	read_checked_line(is, "{");

		std::string line;
		read_line(is, line, "entity count");
		int entityCount;
		try							{ entityCount = lexical_cast<int,std::string>(line); }
		catch(bad_lexical_cast&)	{ throw Exception("The entity count was not a number"); }

		for(int i=0; i<entityCount; ++i)
		{
			entityManager->load_entity(is);
		}

	read_checked_line(is, "}");

	read_checked_line(is, "}");

	return entityManager;
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

	read_checked_line(is, "LightmapPrefix");
	read_checked_line(is, "{");
	read_line(is, lightmapPrefix, "lightmap prefix");
	read_checked_line(is, "}");

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

	read_checked_line(is, "Lightmaps");
	read_checked_line(is, "{");

	std::string line;
	read_line(is, line, "lightmap count");
	int lightmapCount;
	try							{ lightmapCount = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The lightmap count was not an integer"); }

	lightmaps.resize(lightmapCount);
	for(int i=0; i<lightmapCount; ++i)
	{
		lightmaps[i] = BitmapLoader::load_image24(is);
	}

	if(is.get() != '\n') throw Exception("Expected newline after lightmaps");

	read_checked_line(is, "}");

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

	read_checked_line(is, "Lights");
	read_checked_line(is, "{");

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

		if(tokens.size() != 10) throw Exception("Bad light data at line " + lexical_cast<std::string,int>(i));

		std::vector<std::string> positionComponents(&tokens[1], &tokens[4]);
		std::vector<std::string> colourComponents(&tokens[6], &tokens[9]);

		Vector3d position(positionComponents);
		Colour3d colour(colourComponents);
		lights.push_back(Light(position, colour));
	}

	read_checked_line(is, "}");

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

	read_checked_line(is, "Nav");
	read_checked_line(is, "{");

	std::string line;
	read_line(is, line, "navigation dataset count");
	int datasetCount;
	try							{ datasetCount = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The navigation dataset count was not an integer"); }

	for(int i=0; i<datasetCount; ++i)
	{
		read_checked_line(is, "Dataset");
		read_checked_line(is, "{");

		NavMesh_Ptr navMesh = read_navmesh(is);
		AdjacencyList_Ptr adjList = read_adjacency_list(is);
		PathTable_Ptr pathTable = read_path_table(is);

		datasets.push_back(NavDataset_Ptr(new NavDataset(adjList, navMesh, pathTable)));

		read_checked_line(is, "}");
	}

	read_checked_line(is, "}");

	return datasets;
}

/**
Loads an onion tree from the specified std::istream.

@param is	The std::istream
@return		The onion tree
*/
OnionTree_Ptr FileSectionUtil::load_onion_tree_section(std::istream& is)
{
	read_checked_line(is, "OnionTree");
	read_checked_line(is, "{");
	OnionTree_Ptr tree = OnionTree::load_postorder_text(is);
	read_checked_line(is, "}");
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
	read_checked_line(is, "BSPTree");
	read_checked_line(is, "{");
	BSPTree_Ptr tree = BSPTree::load_postorder_text(is);
	read_checked_line(is, "}");
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

	read_checked_line(is, "VisTable");
	read_checked_line(is, "{");

	// Read in the size of the vis table.
	read_line(is, line, "vis table size");
	int size;
	try							{ size = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The vis table size was not an integer"); }

	// Construct an empty vis table of the right size.
	leafVis.reset(new LeafVisTable(size));

	// Read in the vis table itself.
	for(int i=0; i<size; ++i)
	{
		read_line(is, line, "vis table row " + lexical_cast<std::string,int>(i));
		if(line.length() != size) throw Exception("Bad vis table row " + lexical_cast<std::string,int>(i));

		for(int j=0; j<size; ++j)
		{
			if(line[j] == '0') (*leafVis)(i,j) = LEAFVIS_NO;
			else if(line[j] == '1') (*leafVis)(i,j) = LEAFVIS_YES;
			else throw Exception("Bad vis table value in row " + lexical_cast<std::string,int>(i));
		}
	}

	read_checked_line(is, "}");

	return leafVis;
}

//#################### LOADING SUPPORT METHODS ####################
/**
Reads an adjacency list from the specified std::istream.
*/
AdjacencyList_Ptr FileSectionUtil::read_adjacency_list(std::istream& is)
{
	read_checked_line(is, "AdjacencyList");
	read_checked_line(is, "{");

	std::string line;
	read_line(is, line, "adjacency list size");
	int size;
	try							{ size = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The adjacency list size was not an integer"); }

	AdjacencyList_Ptr adjList(new AdjacencyList(size));

	for(int i=0; i<size; ++i)
	{
		read_line(is, line, "adjacency list entry");

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

	read_checked_line(is, "}");

	return adjList;
}

/**
Attempts to read a line from a std::istream into a string and check its validity.

@param is			The std::istream
@param expected		What we expect the line to be
@throws Exception	If EOF is encountered
*/
void FileSectionUtil::read_checked_line(std::istream& is, const std::string& expected)
{
	std::string line;
	read_line(is, line, expected);
	if(line != expected) throw Exception("Expected " + expected);
}

/**
Attempts to read a line from a std::istream into a string.

@param is			The std::istream
@param line			The string into which to read
@param description	A description of what we were trying to read (in case EOF is encountered)
@throws Exception	If EOF is encountered
*/
void FileSectionUtil::read_line(std::istream& is, std::string& line, const std::string& description)
{
	if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read " + description);
}

/**
Reads a navigation mesh from the specified std::istream.
*/
NavMesh_Ptr FileSectionUtil::read_navmesh(std::istream& is)
{
	read_checked_line(is, "Mesh");
	read_checked_line(is, "{");

	// Read in the nav links.
	read_checked_line(is, "Links");
	read_checked_line(is, "{");

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
	read_checked_line(is, "Polygons");
	read_checked_line(is, "{");

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

	read_checked_line(is, "}");

	return NavMesh_Ptr(new NavMesh(navPolygons, navLinks));
}

/**
Reads a (binary format) path table from the specified std::istream.
*/
PathTable_Ptr FileSectionUtil::read_path_table(std::istream& is)
{
	read_checked_line(is, "PathTable");
	read_checked_line(is, "{");

	std::string line;
	read_line(is, line, "path table size");
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

	read_checked_line(is, "}");

	return pathTable;
}

}
