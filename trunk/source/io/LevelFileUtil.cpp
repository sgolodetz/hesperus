/***
 * hesperus: LevelFileUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "LevelFileUtil.h"

#include <fstream>

#include <source/images/BitmapLoader.h>
#include <source/images/BitmapSaver.h>
#include <source/level/LitLevelRenderer.h>
#include <source/level/UnlitLevelRenderer.h>
#include "FileSectionUtil.h"

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads a level from the specified file.

@param filename	The name of the level file
@return			The level
*/
Level_Ptr LevelFileUtil::load(const std::string& filename)
{
	std::ifstream is(filename.c_str(), std::ios_base::binary);
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	std::string fileType;
	if(!std::getline(is, fileType)) throw Exception("Unexpected EOF whilst trying to read file type");

	if(fileType == "HBSPL") return load_lit(is);
	else if(fileType == "HBSPU") return load_unlit(is);
	else throw Exception(filename + " is not a valid level file");
}

//#################### SAVING METHODS ####################
/**
Saves all the relevant pieces of information to the specified level file.

@param filename		The name of the output file
@param polygons		The level polygons
@param tree			The BSP tree for the level
@param portals		The portals for the level
@param leafVis		The leaf visibility table for the level
@param lightmaps	The lightmaps for the level
*/
void LevelFileUtil::save_lit(const std::string& filename, const std::vector<TexturedLitPolygon_Ptr>& polygons,
							 const BSPTree_Ptr& tree, const std::vector<Portal_Ptr>& portals,
							 const LeafVisTable_Ptr& leafVis, const std::vector<Image24_Ptr>& lightmaps)
{
	std::ofstream os(filename.c_str(), std::ios_base::binary);
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");

	os << "HBSPL\n";
	FileSectionUtil::save_polygons_section(os, "Polygons", polygons);
	FileSectionUtil::save_tree_section(os, tree);
	FileSectionUtil::save_polygons_section(os, "Portals", portals);
	FileSectionUtil::save_vis_section(os, leafVis);

	int lightmapCount = static_cast<int>(lightmaps.size());
	for(int i=0; i<lightmapCount; ++i)
	{
		BitmapSaver::save_image24(os, lightmaps[i]);
	}
}

/**
Saves all the relevant pieces of information to the specified level file.

@param filename		The name of the output file
@param polygons		The level polygons
@param tree			The BSP tree for the level
@param portals		The portals for the level
@param leafVis		The leaf visibility table for the level
*/
void LevelFileUtil::save_unlit(const std::string& filename, const std::vector<TexturedPolygon_Ptr>& polygons,
							   const BSPTree_Ptr& tree, const std::vector<Portal_Ptr>& portals,
							   const LeafVisTable_Ptr& leafVis)
{
	std::ofstream os(filename.c_str(), std::ios_base::binary);
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");

	os << "HBSPU\n";
	FileSectionUtil::save_polygons_section(os, "Polygons", polygons);
	FileSectionUtil::save_tree_section(os, tree);
	FileSectionUtil::save_polygons_section(os, "Portals", portals);
	FileSectionUtil::save_vis_section(os, leafVis);
}

//#################### LOADING SUPPORT METHODS ####################
/**
Loads a lit level from the specified std::istream.

@param is	The std::istream
@return		The lit level
*/
Level_Ptr LevelFileUtil::load_lit(std::istream& is)
{
	// Load the data.
	std::vector<TexturedLitPolygon_Ptr> polygons;
	BSPTree_Ptr tree;
	std::vector<Portal_Ptr> portals;
	LeafVisTable_Ptr leafVis;

	FileSectionUtil::load_polygons_section(is, "Polygons", polygons);
	tree = FileSectionUtil::load_tree_section(is);
	FileSectionUtil::load_polygons_section(is, "Portals", portals);
	leafVis = FileSectionUtil::load_vis_section(is);

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
Loads an unlit level from the specified std::istream.

@param is	The std::istream
@return		The unlit level
*/
Level_Ptr LevelFileUtil::load_unlit(std::istream& is)
{
	// Load the data.
	std::vector<TexturedPolygon_Ptr> polygons;
	BSPTree_Ptr tree;
	std::vector<Portal_Ptr> portals;
	LeafVisTable_Ptr leafVis;

	FileSectionUtil::load_polygons_section(is, "Polygons", polygons);
	tree = FileSectionUtil::load_tree_section(is);
	FileSectionUtil::load_polygons_section(is, "Portals", portals);
	leafVis = FileSectionUtil::load_vis_section(is);

	// Construct and return the level.
	LevelRenderer_Ptr levelRenderer(new UnlitLevelRenderer(polygons));
	return Level_Ptr(new Level(levelRenderer, tree, portals, leafVis));
}

}
