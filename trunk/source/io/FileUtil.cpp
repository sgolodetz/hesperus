/***
 * hesperus: FileUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FileUtil.h"

#include <fstream>
#include <sstream>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>
#include <source/images/BitmapLoader.h>
#include <source/images/BitmapSaver.h>
#include <source/level/LitLevelRenderer.h>
#include <source/level/UnlitLevelRenderer.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Loads a level from the specified file.

@param filename	The name of the level file
@return			The level
*/
Level_Ptr FileUtil::load_level_file(const std::string& filename)
{
	std::ifstream is(filename.c_str(), std::ios_base::binary);
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	std::string fileType;
	if(!std::getline(is, fileType)) throw Exception("Unexpected EOF whilst trying to read file type");

	if(fileType == "HBSPL") return load_lit_level_file(is);
	else if(fileType == "HBSPU") return load_unlit_level_file(is);
	else throw Exception(filename + " is not a valid level file");
}

/**
Loads an array of lights from the specified file.

@param filename	The name of the file containing the list of lights
@return			The array of lights
*/
std::vector<Light> FileUtil::load_lights_file(const std::string& filename)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("The lights file could not be read");

	return load_lights_section(is);
}

/**
Loads an empty leaf count and an array of portals from the specified file.

@param filename			The name of the portals file
@param emptyLeafCount	Used to return the empty leaf count to the caller
@param portals			Used to return the portals to the caller
*/
void FileUtil::load_portals_file(const std::string& filename, int& emptyLeafCount, std::vector<Portal_Ptr>& portals)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("The portals file could not be read");

	std::string line;
	if(!std::getline(is, line)) throw Exception("The empty leaf count could not be read");
	try							{ emptyLeafCount = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The empty leaf count was not an integer"); }

	load_polygons_section(is, portals);
}

/**
Loads a leaf visibility table from the specified file.

@param filename	The name of the file from which to load the visibility table
@return			The visibility table
*/
LeafVisTable_Ptr FileUtil::load_vis_file(const std::string& filename)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("The vis file could not be read");

	return load_vis_section(is);
}

/**
Saves all the relevant pieces of information to the specified level file.

@param filename		The name of the output file
@param polygons		The level polygons
@param tree			The BSP tree for the level
@param portals		The portals for the level
@param leafVis		The leaf visibility table for the level
*/
void FileUtil::save_level_file(const std::string& filename, const std::vector<TexturedPolygon_Ptr>& polygons,
							   const BSPTree_Ptr& tree, const std::vector<Portal_Ptr>& portals,
							   const LeafVisTable_Ptr& leafVis)
{
	std::ofstream os(filename.c_str(), std::ios_base::binary);
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");

	os << "HBSPU\n";
	write_polygons(os, polygons);
	os << "***\n";
	tree->output_postorder_text(os);
	os << "***\n";
	write_polygons(os, portals);
	os << "***\n";
	save_vis_section(os, leafVis);
}

/**
Saves all the relevant pieces of information to the specified level file.

@param filename		The name of the output file
@param polygons		The level polygons
@param tree			The BSP tree for the level
@param portals		The portals for the level
@param leafVis		The leaf visibility table for the level
@param lightmaps	The lightmaps for the level
*/
void FileUtil::save_level_file(const std::string& filename, const std::vector<TexturedLitPolygon_Ptr>& polygons,
							   const BSPTree_Ptr& tree, const std::vector<Portal_Ptr>& portals,
							   const LeafVisTable_Ptr& leafVis, const std::vector<Image24_Ptr>& lightmaps)
{
	std::ofstream os(filename.c_str(), std::ios_base::binary);
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");

	os << "HBSPL\n";
	write_polygons(os, polygons);
	os << "***\n";
	tree->output_postorder_text(os);
	os << "***\n";
	write_polygons(os, portals);
	os << "***\n";
	save_vis_section(os, leafVis);
	os << "***\n";

	int lightmapCount = static_cast<int>(lightmaps.size());
	for(int i=0; i<lightmapCount; ++i)
	{
		BitmapSaver::save_image24(os, lightmaps[i]);
	}
}

/**
Saves a leaf visibility table to a std::ostream.

@param os		The std::ostream
@param leafVis	The leaf visibility table
*/
void FileUtil::save_vis_section(std::ostream& os, const LeafVisTable_Ptr& leafVis)
{
	const LeafVisTable& table = *leafVis;

	int size = table.size();
	os << size << '\n';
	for(int i=0; i<size; ++i)
	{
		for(int j=0; j<size; ++j)
		{
			switch(table(i,j))
			{
				case LEAFVIS_NO:
				{
					os << '0';
					break;
				}
				case LEAFVIS_YES:
				{
					os << '1';
					break;
				}
			}
		}
		os << '\n';
	}
}

//#################### PRIVATE METHODS ####################
/**
Reads the lightmap prefix from the specified std::istream.

@param is			The std::istream
@return				The lightmap prefix
@throws Exception	If EOF is encountered whilst trying to read the lightmap prefix
*/
std::string FileUtil::load_lightmap_prefix_section(std::istream& is)
{
	std::string lightmapPrefix;
	if(!std::getline(is, lightmapPrefix)) throw Exception("Unexpected EOF whilst trying to read lightmap prefix");
	return lightmapPrefix;
}

/**
Loads an array of lights from the specified std::istream.

@param is	The std::istream
@return		The array of lights
*/
std::vector<Light> FileUtil::load_lights_section(std::istream& is)
{
	std::vector<Light> lights;

	// Read in the light count.
	int lightCount;

	std::string line;
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

	return lights;
}

/**
Loads a lit level from the specified std::istream.

@param is	The std::istream
@return		The lit level
*/
Level_Ptr FileUtil::load_lit_level_file(std::istream& is)
{
	// Load the data.
	std::vector<TexturedLitPolygon_Ptr> polygons;
	BSPTree_Ptr tree;
	std::vector<Portal_Ptr> portals;
	LeafVisTable_Ptr leafVis;

	load_polygons_section(is, polygons);
	load_separator(is);
	tree = load_tree_section(is);
	load_separator(is);
	load_polygons_section(is, portals);
	load_separator(is);
	leafVis = load_vis_section(is);
	load_separator(is);

	int polyCount = static_cast<int>(polygons.size());
	std::vector<Image24_Ptr> lightmaps(polyCount);
	for(int i=0; i<polyCount; ++i)
	{
		lightmaps[i] = BitmapLoader::load_image24(is);
	}

	// Construct and return the level.
	LevelRenderer_Ptr levelRenderer(new LitLevelRenderer(polygons, lightmaps));
	return Level_Ptr(new Level(levelRenderer, tree, portals, leafVis));
}

/**
Loads a separator (a line containing only ***) from the specified std::istream.

@param is	The std::istream
*/
void FileUtil::load_separator(std::istream& is)
{
	std::string line;
	if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read separator");
	if(line != "***") throw Exception("Bad separator");
}

/**
Loads a tree from the specified std::istream.

@param is	The std::istream
@return		The tree
*/
BSPTree_Ptr FileUtil::load_tree_section(std::istream& is)
{
	return BSPTree::load_postorder_text(is);
}

/**
Loads an unlit level from the specified std::istream.

@param is	The std::istream
@return		The unlit level
*/
Level_Ptr FileUtil::load_unlit_level_file(std::istream& is)
{
	// Load the data.
	std::vector<TexturedPolygon_Ptr> polygons;
	BSPTree_Ptr tree;
	std::vector<Portal_Ptr> portals;
	LeafVisTable_Ptr leafVis;

	load_polygons_section(is, polygons);
	load_separator(is);
	tree = load_tree_section(is);
	load_separator(is);
	load_polygons_section(is, portals);
	load_separator(is);
	leafVis = load_vis_section(is);

	// Construct and return the level.
	LevelRenderer_Ptr levelRenderer(new UnlitLevelRenderer(polygons));
	return Level_Ptr(new Level(levelRenderer, tree, portals, leafVis));
}

/**
Loads a leaf visibility table from the specified std::istream.

@param is	The std::istream
@return		The visibility table
*/
LeafVisTable_Ptr FileUtil::load_vis_section(std::istream& is)
{
	LeafVisTable_Ptr leafVis;

	std::string line;

	// Read in the size of the vis table.
	int size;
	if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read vis table size");
	try							{ size = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The vis table size was not an integer"); }

	// Construct an empty vis table of the right size.
	leafVis.reset(new LeafVisTable(size));

	// Read in the vis table itself.
	for(int i=0; i<size; ++i)
	{
		if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read vis table row " + lexical_cast<std::string,int>(i));
		if(line.length() != size) throw Exception("Bad vis table row " + lexical_cast<std::string,int>(i));

		for(int j=0; j<size; ++j)
		{
			if(line[j] == '0') (*leafVis)(i,j) = LEAFVIS_NO;
			else if(line[j] == '1') (*leafVis)(i,j) = LEAFVIS_YES;
			else throw Exception("Bad vis table value in row " + lexical_cast<std::string,int>(i));
		}
	}

	return leafVis;
}

}
