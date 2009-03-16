/***
 * hesperus: LevelFileUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "LevelFileUtil.h"

#include <fstream>

#include <boost/filesystem/operations.hpp>
namespace bf = boost::filesystem;

#include <source/images/BitmapLoader.h>
#include <source/images/BitmapSaver.h>
#include <source/level/LitGeometryRenderer.h>
#include <source/level/UnlitGeometryRenderer.h>
#include "DirectoryFinder.h"
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

@param filename			The name of the output file
@param polygons			The level polygons
@param tree				The BSP tree for the level
@param portals			The portals for the level
@param leafVis			The leaf visibility table for the level
@param lightmaps		The lightmaps for the level
@param onionPolygons	The polygons for the onion tree
@param onionTree		The onion tree for the level
@param onionPortals		The onion portals for the level
@param navDatasets		The navigation datasets for the level
*/
void LevelFileUtil::save_lit(const std::string& filename,
							 const std::vector<TexturedLitPolygon_Ptr>& polygons, const BSPTree_Ptr& tree,
							 const std::vector<Portal_Ptr>& portals,
							 const LeafVisTable_Ptr& leafVis,
							 const std::vector<Image24_Ptr>& lightmaps,
							 const std::vector<CollisionPolygon_Ptr>& onionPolygons, const OnionTree_Ptr& onionTree,
							 const std::vector<OnionPortal_Ptr>& onionPortals,
							 const std::vector<NavDataset_Ptr>& navDatasets,
							 const EntityManager_Ptr& entityManager)
{
	std::ofstream os(filename.c_str(), std::ios_base::binary);
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");

	os << "HBSPL\n";
	FileSectionUtil::save_polygons_section(os, "Polygons", polygons);
	FileSectionUtil::save_tree_section(os, tree);
	FileSectionUtil::save_polygons_section(os, "Portals", portals);
	FileSectionUtil::save_vis_section(os, leafVis);
	FileSectionUtil::save_lightmaps_section(os, lightmaps);
	FileSectionUtil::save_polygons_section(os, "OnionPolygons", onionPolygons);
	FileSectionUtil::save_onion_tree_section(os, onionTree);
	FileSectionUtil::save_polygons_section(os, "OnionPortals", onionPortals);
	FileSectionUtil::save_nav_section(os, navDatasets);
	FileSectionUtil::save_entities_section(os, entityManager);
}

/**
Saves all the relevant pieces of information to the specified level file.

@param filename			The name of the output file
@param polygons			The level polygons
@param tree				The BSP tree for the level
@param portals			The portals for the level
@param leafVis			The leaf visibility table for the level
@param onionPolygons	The polygons for the onion tree
@param onionTree		The onion tree for the level
@param onionPortals		The onion portals for the level
@param navDatasets		The navigation datasets for the level
*/
void LevelFileUtil::save_unlit(const std::string& filename,
							   const std::vector<TexturedPolygon_Ptr>& polygons, const BSPTree_Ptr& tree,
							   const std::vector<Portal_Ptr>& portals,
							   const LeafVisTable_Ptr& leafVis,
							   const std::vector<CollisionPolygon_Ptr>& onionPolygons, const OnionTree_Ptr& onionTree,
							   const std::vector<OnionPortal_Ptr>& onionPortals,
							   const std::vector<NavDataset_Ptr>& navDatasets,
							   const EntityManager_Ptr& entityManager)
{
	std::ofstream os(filename.c_str(), std::ios_base::binary);
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");

	os << "HBSPU\n";
	FileSectionUtil::save_polygons_section(os, "Polygons", polygons);
	FileSectionUtil::save_tree_section(os, tree);
	FileSectionUtil::save_polygons_section(os, "Portals", portals);
	FileSectionUtil::save_vis_section(os, leafVis);
	FileSectionUtil::save_polygons_section(os, "OnionPolygons", onionPolygons);
	FileSectionUtil::save_onion_tree_section(os, onionTree);
	FileSectionUtil::save_polygons_section(os, "OnionPortals", onionPortals);
	FileSectionUtil::save_nav_section(os, navDatasets);
	FileSectionUtil::save_entities_section(os, entityManager);
}

//#################### LOADING SUPPORT METHODS ####################
/**
Loads a lit level from the specified std::istream.

@param is	The std::istream
@return		The lit level
*/
Level_Ptr LevelFileUtil::load_lit(std::istream& is)
{
	std::vector<TexturedLitPolygon_Ptr> polygons;
	BSPTree_Ptr tree;
	std::vector<Portal_Ptr> portals;
	LeafVisTable_Ptr leafVis;
	std::vector<CollisionPolygon_Ptr> onionPolygons;
	OnionTree_Ptr onionTree;
	std::vector<OnionPortal_Ptr> onionPortals;
	std::vector<NavDataset_Ptr> navDatasets;
	EntityManager_Ptr entityManager;

	// Load the data.
	FileSectionUtil::load_polygons_section(is, "Polygons", polygons);
	tree = FileSectionUtil::load_tree_section(is);
	FileSectionUtil::load_polygons_section(is, "Portals", portals);
	leafVis = FileSectionUtil::load_vis_section(is);
	std::vector<Image24_Ptr> lightmaps = FileSectionUtil::load_lightmaps_section(is);
	FileSectionUtil::load_polygons_section(is, "OnionPolygons", onionPolygons);
	onionTree = FileSectionUtil::load_onion_tree_section(is);
	FileSectionUtil::load_polygons_section(is, "OnionPortals", onionPortals);
	navDatasets = FileSectionUtil::load_nav_section(is);

	bf::path settingsDir = determine_settings_directory(determine_base_directory_from_game());
	entityManager = FileSectionUtil::load_entities_section(is, settingsDir);

	// Construct and return the level.
	GeometryRenderer_Ptr geomRenderer(new LitGeometryRenderer(polygons, lightmaps));
	return Level_Ptr(new Level(geomRenderer, tree, portals, leafVis, onionPolygons, onionTree, onionPortals, navDatasets, entityManager));
}

/**
Loads an unlit level from the specified std::istream.

@param is	The std::istream
@return		The unlit level
*/
Level_Ptr LevelFileUtil::load_unlit(std::istream& is)
{
	std::vector<TexturedPolygon_Ptr> polygons;
	BSPTree_Ptr tree;
	std::vector<Portal_Ptr> portals;
	LeafVisTable_Ptr leafVis;
	std::vector<CollisionPolygon_Ptr> onionPolygons;
	OnionTree_Ptr onionTree;
	std::vector<OnionPortal_Ptr> onionPortals;
	std::vector<NavDataset_Ptr> navDatasets;
	EntityManager_Ptr entityManager;

	// Load the data.
	FileSectionUtil::load_polygons_section(is, "Polygons", polygons);
	tree = FileSectionUtil::load_tree_section(is);
	FileSectionUtil::load_polygons_section(is, "Portals", portals);
	leafVis = FileSectionUtil::load_vis_section(is);
	FileSectionUtil::load_polygons_section(is, "OnionPolygons", onionPolygons);
	onionTree = FileSectionUtil::load_onion_tree_section(is);
	FileSectionUtil::load_polygons_section(is, "OnionPortals", onionPortals);
	navDatasets = FileSectionUtil::load_nav_section(is);

	bf::path settingsDir = determine_settings_directory(determine_base_directory_from_game());
	entityManager = FileSectionUtil::load_entities_section(is, settingsDir);

	// Construct and return the level.
	GeometryRenderer_Ptr geomRenderer(new UnlitGeometryRenderer(polygons));
	return Level_Ptr(new Level(geomRenderer, tree, portals, leafVis, onionPolygons, onionTree, onionPortals, navDatasets, entityManager));
}

}
