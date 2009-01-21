/***
 * hesperus: FileSectionUtil_Load.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FileSectionUtil.h"

#include <boost/lexical_cast.hpp>
namespace bf = boost::filesystem;
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include "EntDefFileUtil.h"

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
	std::string line, lightmapPrefix;

	read_checked_line(is, "LightmapPrefix");
	read_checked_line(is, "{");
	read_line(is, lightmapPrefix, "lightmap prefix");
	read_checked_line(is, "}");

	return lightmapPrefix;
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

}
