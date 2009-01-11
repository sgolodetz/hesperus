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

	FileSectionUtil::load_polygons_section(is, "Portals", portals);
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

	return FileSectionUtil::load_vis_section(is);
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
	FileSectionUtil::save_polygons_section(os, "Polygons", polygons);
	os << "***\n";
	FileSectionUtil::save_tree_section(os, tree);
	os << "***\n";
	FileSectionUtil::save_polygons_section(os, "Portals", portals);
	os << "***\n";
	FileSectionUtil::save_vis_section(os, leafVis);
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
	FileSectionUtil::save_polygons_section(os, "Polygons", polygons);
	os << "***\n";
	FileSectionUtil::save_tree_section(os, tree);
	os << "***\n";
	FileSectionUtil::save_polygons_section(os, "Portals", portals);
	os << "***\n";
	FileSectionUtil::save_vis_section(os, leafVis);
	os << "***\n";

	int lightmapCount = static_cast<int>(lightmaps.size());
	for(int i=0; i<lightmapCount; ++i)
	{
		BitmapSaver::save_image24(os, lightmaps[i]);
	}
}

//#################### PRIVATE METHODS ####################
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

	FileSectionUtil::load_polygons_section(is, "Polygons", polygons);
	load_separator(is);
	tree = FileSectionUtil::load_tree_section(is);
	load_separator(is);
	FileSectionUtil::load_polygons_section(is, "Portals", portals);
	load_separator(is);
	leafVis = FileSectionUtil::load_vis_section(is);
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

	FileSectionUtil::load_polygons_section(is, "Polygons", polygons);
	load_separator(is);
	tree = FileSectionUtil::load_tree_section(is);
	load_separator(is);
	FileSectionUtil::load_polygons_section(is, "Portals", portals);
	load_separator(is);
	leafVis = FileSectionUtil::load_vis_section(is);

	// Construct and return the level.
	LevelRenderer_Ptr levelRenderer(new UnlitLevelRenderer(polygons));
	return Level_Ptr(new Level(levelRenderer, tree, portals, leafVis));
}

}
