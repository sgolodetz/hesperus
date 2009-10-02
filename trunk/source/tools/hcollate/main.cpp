/***
 * hcollate: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem/operations.hpp>
#include <boost/lexical_cast.hpp>
namespace bf = boost::filesystem;
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>
#include <source/images/PNGLoader.h>
#include <source/io/files/DefinitionsFile.h>
#include <source/io/files/DefinitionsSpecifierFile.h>
#include <source/io/files/LevelFile.h>
#include <source/io/files/LitTreeFile.h>
#include <source/io/files/NavFile.h>
#include <source/io/files/ObjectsFile.h>
#include <source/io/files/OnionPortalsFile.h>
#include <source/io/files/OnionTreeFile.h>
#include <source/io/files/PortalsFile.h>
#include <source/io/files/TreeFile.h>
#include <source/io/files/VisFile.h>
#include <source/io/util/DirectoryFinder.h>
#include <source/level/objects/base/ComponentPropertyTypeMap.h>
#include <source/level/objects/base/ObjectSpecification.h>
#include <source/util/PolygonTypes.h>
using namespace hesp;

namespace hesp {

boost::filesystem::path determine_base_directory()
{
	return determine_base_directory_from_tool();
}

}

//#################### FUNCTIONS ####################
void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: hcollate {+L <input lit tree> | -L <input tree>} <input portals> <input vis> <input onion tree> <input onion portals> <input nav data> <input definitions specifier file> <input objects> <output filename>" << std::endl;
	exit(EXIT_FAILURE);
}

void collate_lit(const std::string& treeFilename, const std::string& portalsFilename, const std::string& visFilename,
				 const std::string& onionTreeFilename, const std::string& onionPortalsFilename,
				 const std::string& navFilename, const std::string& definitionsSpecifierFilename,
				 const std::string& objectsFilename, const std::string& outputFilename)
try
{
	// Load the lit polygons, tree and lightmap prefix.
	typedef std::vector<TexturedLitPolygon_Ptr> TexLitPolyVector;
	TexLitPolyVector polygons;
	BSPTree_Ptr tree;
	std::string lightmapPrefix;
	LitTreeFile::load(treeFilename, polygons, tree, lightmapPrefix);

	// Load the portals.
	int emptyLeafCount;
	std::vector<Portal_Ptr> portals;
	PortalsFile::load(portalsFilename, emptyLeafCount, portals);

	// Load the vis table.
	LeafVisTable_Ptr leafVis = VisFile::load(visFilename);

	// Load the lightmaps.
	int polyCount = static_cast<int>(polygons.size());
	std::vector<Image24_Ptr> lightmaps(polyCount);
	for(int i=0; i<polyCount; ++i)
	{
		std::string filename = lightmapPrefix + lexical_cast<std::string,int>(i) + ".png";
		lightmaps[i] = PNGLoader::load_image24(filename);
	}

	// Load the onion tree.
	typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;
	ColPolyVector onionPolygons;
	OnionTree_Ptr onionTree;
	OnionTreeFile::load(onionTreeFilename, onionPolygons, onionTree);

	// Load the onion portals.
	std::vector<OnionPortal_Ptr> onionPortals = OnionPortalsFile::load(onionPortalsFilename);

	// Load the navigation data.
	std::vector<NavDataset_Ptr> navDatasets = NavFile::load(navFilename);

	// Load the definitions specifier.
	std::string definitionsFilename = DefinitionsSpecifierFile::load(definitionsSpecifierFilename);

	// Load the object bounds and component property types from the definitions file.
	bf::path baseDir = determine_base_directory();
	bf::path settingsDir = determine_settings_directory(baseDir);
	BoundsManager_Ptr boundsManager;
	ComponentPropertyTypeMap componentPropertyTypes;
	std::map<std::string,ObjectSpecification> archetypes;
	DefinitionsFile::load((settingsDir / definitionsFilename).file_string(), boundsManager, componentPropertyTypes, archetypes);

	// Load the objects.
	ObjectManager_Ptr objectManager = ObjectsFile::load(objectsFilename, boundsManager, componentPropertyTypes, archetypes, baseDir);

	// Write everything to the output file.
	LevelFile::save_lit(outputFilename,
						polygons, tree,
						portals,
						leafVis,
						lightmaps,
						onionPolygons, onionTree,
						onionPortals,
						navDatasets,
						definitionsFilename,
						objectManager);
}
catch(Exception& e) { quit_with_error(e.cause()); }

void collate_unlit(const std::string& treeFilename, const std::string& portalsFilename, const std::string& visFilename,
				   const std::string& onionTreeFilename, const std::string& onionPortalsFilename,
				   const std::string& navFilename, const std::string& definitionsSpecifierFilename,
				   const std::string& objectsFilename, const std::string& outputFilename)
try
{
	// Load the unlit polygons and tree.
	typedef std::vector<TexturedPolygon_Ptr> TexPolyVector;
	TexPolyVector polygons;
	BSPTree_Ptr tree;
	TreeFile::load(treeFilename, polygons, tree);

	// Load the portals.
	int emptyLeafCount;
	std::vector<Portal_Ptr> portals;
	PortalsFile::load(portalsFilename, emptyLeafCount, portals);

	// Load the vis table.
	LeafVisTable_Ptr leafVis = VisFile::load(visFilename);

	// Load the onion tree.
	typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;
	ColPolyVector onionPolygons;
	OnionTree_Ptr onionTree;
	OnionTreeFile::load(onionTreeFilename, onionPolygons, onionTree);

	// Load the onion portals.
	std::vector<OnionPortal_Ptr> onionPortals = OnionPortalsFile::load(onionPortalsFilename);

	// Load the navigation data.
	std::vector<NavDataset_Ptr> navDatasets = NavFile::load(navFilename);

	// Load the definitions specifier.
	std::string definitionsFilename = DefinitionsSpecifierFile::load(definitionsSpecifierFilename);

	// Load the object bounds and component property types from the definitions file.
	bf::path baseDir = determine_base_directory();
	bf::path settingsDir = determine_settings_directory(baseDir);
	BoundsManager_Ptr boundsManager;
	ComponentPropertyTypeMap componentPropertyTypes;
	std::map<std::string,ObjectSpecification> archetypes;
	DefinitionsFile::load((settingsDir / definitionsFilename).file_string(), boundsManager, componentPropertyTypes, archetypes);

	// Load the objects.
	ObjectManager_Ptr objectManager = ObjectsFile::load(objectsFilename, boundsManager, componentPropertyTypes, archetypes, baseDir);

	// Write everything to the output file.
	LevelFile::save_unlit(outputFilename,
						  polygons, tree,
						  portals,
						  leafVis,
						  onionPolygons, onionTree,
						  onionPortals,
						  navDatasets,
						  definitionsFilename,
						  objectManager);
}
catch(Exception& e) { quit_with_error(e.cause()); }

int main(int argc, char *argv[])
{
	if(argc != 11) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);

	if(args[1] == "+L") collate_lit(args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10]);
	else if(args[1] == "-L") collate_unlit(args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9], args[10]);
	else quit_with_usage();

	return 0;
}
