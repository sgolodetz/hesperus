/***
 * hesperus: LevelFile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "LevelFile.h"

#include <fstream>

#include <boost/filesystem/operations.hpp>
namespace bf = boost::filesystem;

#include <source/io/sections/EntitiesSection.h>
#include <source/io/sections/LightmapsSection.h>
#include <source/io/sections/NavSection.h>
#include <source/io/sections/OnionTreeSection.h>
#include <source/io/sections/PolygonsSection.h>
#include <source/io/sections/TreeSection.h>
#include <source/io/sections/VisSection.h>
#include <source/io/util/DirectoryFinder.h>
#include <source/level/LitGeometryRenderer.h>
#include <source/level/UnlitGeometryRenderer.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads a level from the specified file.

@param filename	The name of the level file
@return			The level
*/
Level_Ptr LevelFile::load(const std::string& filename)
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
void LevelFile::save_lit(const std::string& filename,
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
	PolygonsSection::save(os, "Polygons", polygons);
	TreeSection::save(os, tree);
	PolygonsSection::save(os, "Portals", portals);
	VisSection::save(os, leafVis);
	LightmapsSection::save(os, lightmaps);
	PolygonsSection::save(os, "OnionPolygons", onionPolygons);
	OnionTreeSection::save(os, onionTree);
	PolygonsSection::save(os, "OnionPortals", onionPortals);
	NavSection::save(os, navDatasets);
	EntitiesSection::save(os, entityManager);
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
void LevelFile::save_unlit(const std::string& filename,
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
	PolygonsSection::save(os, "Polygons", polygons);
	TreeSection::save(os, tree);
	PolygonsSection::save(os, "Portals", portals);
	VisSection::save(os, leafVis);
	PolygonsSection::save(os, "OnionPolygons", onionPolygons);
	OnionTreeSection::save(os, onionTree);
	PolygonsSection::save(os, "OnionPortals", onionPortals);
	NavSection::save(os, navDatasets);
	EntitiesSection::save(os, entityManager);
}

//#################### LOADING SUPPORT METHODS ####################
/**
Loads a lit level from the specified std::istream.

@param is	The std::istream
@return		The lit level
*/
Level_Ptr LevelFile::load_lit(std::istream& is)
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
	ModelManager_Ptr modelManager;

	// Load the level data.
	PolygonsSection::load(is, "Polygons", polygons);
	tree = TreeSection::load(is);
	PolygonsSection::load(is, "Portals", portals);
	leafVis = VisSection::load(is);
	std::vector<Image24_Ptr> lightmaps = LightmapsSection::load(is);
	PolygonsSection::load(is, "OnionPolygons", onionPolygons);
	onionTree = OnionTreeSection::load(is);
	PolygonsSection::load(is, "OnionPortals", onionPortals);
	navDatasets = NavSection::load(is);

	bf::path baseDir = determine_base_directory_from_game();
	entityManager = EntitiesSection::load(is, baseDir);

	// Load the models.
	modelManager = load_models(entityManager);

	// Construct and return the level.
	GeometryRenderer_Ptr geomRenderer(new LitGeometryRenderer(polygons, lightmaps));
	return Level_Ptr(new Level(geomRenderer, tree, portals, leafVis, onionPolygons, onionTree, onionPortals, navDatasets, entityManager, modelManager));
}

/**
Loads the models necessary for the animatable entities in the specified entity manager.

@param entityManager	The entity manager
@return					A model manager containing the loaded models
*/
ModelManager_Ptr LevelFile::load_models(const EntityManager_Ptr& entityManager)
{
	ModelManager_Ptr modelManager(new ModelManager);

	const std::vector<Entity_Ptr>& animatables = entityManager->group("Animatables");
	int animatableCount = static_cast<int>(animatables.size());
	for(int i=0; i<animatableCount; ++i)
	{
		modelManager->register_model(animatables[i]->character_model());
	}

	modelManager->load_all();

	return modelManager;
}

/**
Loads an unlit level from the specified std::istream.

@param is	The std::istream
@return		The unlit level
*/
Level_Ptr LevelFile::load_unlit(std::istream& is)
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
	ModelManager_Ptr modelManager;

	// Load the level data.
	PolygonsSection::load(is, "Polygons", polygons);
	tree = TreeSection::load(is);
	PolygonsSection::load(is, "Portals", portals);
	leafVis = VisSection::load(is);
	PolygonsSection::load(is, "OnionPolygons", onionPolygons);
	onionTree = OnionTreeSection::load(is);
	PolygonsSection::load(is, "OnionPortals", onionPortals);
	navDatasets = NavSection::load(is);

	bf::path baseDir = determine_base_directory_from_game();
	entityManager = EntitiesSection::load(is, baseDir);

	// Load the models.
	modelManager = load_models(entityManager);

	// Construct and return the level.
	GeometryRenderer_Ptr geomRenderer(new UnlitGeometryRenderer(polygons));
	return Level_Ptr(new Level(geomRenderer, tree, portals, leafVis, onionPolygons, onionTree, onionPortals, navDatasets, entityManager, modelManager));
}

}
