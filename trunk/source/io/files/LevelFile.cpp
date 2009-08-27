/***
 * hesperus: LevelFile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "LevelFile.h"

#include <fstream>

#include <boost/filesystem/operations.hpp>
namespace bf = boost::filesystem;

#include <source/io/files/DefinitionsFile.h>
#include <source/io/sections/DefinitionsSpecifierSection.h>
#include <source/io/sections/LightmapsSection.h>
#include <source/io/sections/NavSection.h>
#include <source/io/sections/ObjectsSection.h>
#include <source/io/sections/OnionTreeSection.h>
#include <source/io/sections/PolygonsSection.h>
#include <source/io/sections/TreeSection.h>
#include <source/io/sections/VisSection.h>
#include <source/io/util/DirectoryFinder.h>
#include <source/level/LitGeometryRenderer.h>
#include <source/level/UnlitGeometryRenderer.h>
#include <source/level/objects/components/ICmpModelRender.h>

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

@param filename					The name of the output file
@param polygons					The level polygons
@param tree						The BSP tree for the level
@param portals					The portals for the level
@param leafVis					The leaf visibility table for the level
@param lightmaps				The lightmaps for the level
@param onionPolygons			The polygons for the onion tree
@param onionTree				The onion tree for the level
@param onionPortals				The onion portals for the level
@param navDatasets				The navigation datasets for the level
@param definitionsFilename		The name of the definitions file for the level
@param objectManager			The object manager containing the objects for the level
*/
void LevelFile::save_lit(const std::string& filename,
						 const std::vector<TexturedLitPolygon_Ptr>& polygons, const BSPTree_CPtr& tree,
						 const std::vector<Portal_Ptr>& portals,
						 const LeafVisTable_CPtr& leafVis,
						 const std::vector<Image24_Ptr>& lightmaps,
						 const std::vector<CollisionPolygon_Ptr>& onionPolygons, const OnionTree_CPtr& onionTree,
						 const std::vector<OnionPortal_Ptr>& onionPortals,
						 const std::vector<NavDataset_Ptr>& navDatasets,
						 const std::string& definitionsFilename,
						 const ObjectManager_Ptr& objectManager)
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
	DefinitionsSpecifierSection::save(os, definitionsFilename);
	ObjectsSection::save(os, objectManager);
}

/**
Saves all the relevant pieces of information to the specified level file.

@param filename					The name of the output file
@param polygons					The level polygons
@param tree						The BSP tree for the level
@param portals					The portals for the level
@param leafVis					The leaf visibility table for the level
@param onionPolygons			The polygons for the onion tree
@param onionTree				The onion tree for the level
@param onionPortals				The onion portals for the level
@param navDatasets				The navigation datasets for the level
@param definitionsFilename		The name of the definitions file for the level
@param objectManager			The object manager containing the objects for the level
*/
void LevelFile::save_unlit(const std::string& filename,
						   const std::vector<TexturedPolygon_Ptr>& polygons, const BSPTree_CPtr& tree,
						   const std::vector<Portal_Ptr>& portals,
						   const LeafVisTable_CPtr& leafVis,
						   const std::vector<CollisionPolygon_Ptr>& onionPolygons, const OnionTree_CPtr& onionTree,
						   const std::vector<OnionPortal_Ptr>& onionPortals,
						   const std::vector<NavDataset_Ptr>& navDatasets,
						   const std::string& definitionsFilename,
						   const ObjectManager_Ptr& objectManager)
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
	DefinitionsSpecifierSection::save(os, definitionsFilename);
	ObjectsSection::save(os, objectManager);
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
	std::string definitionsFilename;
	ObjectManager_Ptr objectManager;
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
	definitionsFilename = DefinitionsSpecifierSection::load(is);

	bf::path baseDir = determine_base_directory_from_game();
	bf::path settingsDir = determine_settings_directory(baseDir);
	std::vector<AABB3d> aabbs;
	std::map<std::string,std::map<std::string,std::string> > componentPropertyTypes;
	DefinitionsFile::load((settingsDir / definitionsFilename).file_string(), aabbs, componentPropertyTypes);
	objectManager = ObjectsSection::load(is, aabbs, componentPropertyTypes, baseDir);

	// Load the models.
	modelManager = load_models(objectManager);

	// Construct and return the level.
	GeometryRenderer_Ptr geomRenderer(new LitGeometryRenderer(polygons, lightmaps));
	return Level_Ptr(new Level(geomRenderer, tree, portals, leafVis, onionPolygons, onionTree, onionPortals, navDatasets, objectManager, modelManager));
}

/**
Loads the models necessary for the objects in the specified object manager.

@param objectManager	The object manager
@return					A model manager containing the loaded models
*/
ModelManager_Ptr LevelFile::load_models(const ObjectManager_Ptr& objectManager)
{
	ModelManager_Ptr modelManager(new ModelManager);

	std::vector<ObjectID> modelContainers = objectManager->group("ModelContainers");
	for(size_t i=0, size=modelContainers.size(); i<size; ++i)
	{
		ICmpModelRender_Ptr cmpRender = objectManager->get_component(modelContainers[i], cmpRender);

		// Note: Setting the model manager automatically registers the model inside the component with it.
		cmpRender->set_model_manager(modelManager);
	}

	modelManager->load_all();

	// Now that all the models have been loaded, set the skeletons for the animation controllers.
	for(size_t i=0, size=modelContainers.size(); i<size; ++i)
	{
		ICmpModelRender_Ptr cmpRender = objectManager->get_component(modelContainers[i], cmpRender);
		cmpRender->set_skeleton();
	}

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
	std::string definitionsFilename;
	ObjectManager_Ptr objectManager;
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
	definitionsFilename = DefinitionsSpecifierSection::load(is);

	bf::path baseDir = determine_base_directory_from_game();
	bf::path settingsDir = determine_settings_directory(baseDir);
	std::vector<AABB3d> aabbs;
	std::map<std::string,std::map<std::string,std::string> > componentPropertyTypes;
	DefinitionsFile::load((settingsDir / definitionsFilename).file_string(), aabbs, componentPropertyTypes);
	objectManager = ObjectsSection::load(is, aabbs, componentPropertyTypes, baseDir);

	// Load the models.
	modelManager = load_models(objectManager);

	// Construct and return the level.
	GeometryRenderer_Ptr geomRenderer(new UnlitGeometryRenderer(polygons));
	return Level_Ptr(new Level(geomRenderer, tree, portals, leafVis, onionPolygons, onionTree, onionPortals, navDatasets, objectManager, modelManager));
}

}
