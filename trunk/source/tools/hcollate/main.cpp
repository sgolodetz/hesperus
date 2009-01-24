/***
 * hcollate: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>
#include <source/images/BitmapLoader.h>
#include <source/io/LevelFileUtil.h>
#include <source/io/LitTreeFileUtil.h>
#include <source/io/NavFileUtil.h>
#include <source/io/OnionPortalsFileUtil.h>
#include <source/io/OnionTreeFileUtil.h>
#include <source/io/PortalsFileUtil.h>
#include <source/io/TreeFileUtil.h>
#include <source/io/VisFileUtil.h>
#include <source/util/PolygonTypes.h>
using namespace hesp;

//#################### FUNCTIONS ####################
void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: hcollate {+L <input lit tree> | -L <input tree>} <input portals> <input vis> <input onion tree> <input onion portals> <input nav data> <input entities> <output filename>" << std::endl;
	exit(EXIT_FAILURE);
}

void collate_lit(const std::string& treeFilename, const std::string& portalsFilename, const std::string& visFilename,
				 const std::string& onionTreeFilename, const std::string& onionPortalsFilename,
				 const std::string& navFilename, const std::string& entitiesFilename, const std::string& outputFilename)
try
{
	// Load the lit polygons, tree and lightmap prefix.
	typedef std::vector<TexturedLitPolygon_Ptr> TexLitPolyVector;
	TexLitPolyVector polygons;
	BSPTree_Ptr tree;
	std::string lightmapPrefix;
	LitTreeFileUtil::load(treeFilename, polygons, tree, lightmapPrefix);

	// Load the portals.
	int emptyLeafCount;
	std::vector<Portal_Ptr> portals;
	PortalsFileUtil::load(portalsFilename, emptyLeafCount, portals);

	// Load the vis table.
	LeafVisTable_Ptr leafVis = VisFileUtil::load(visFilename);

	// Load the lightmaps.
	int polyCount = static_cast<int>(polygons.size());
	std::vector<Image24_Ptr> lightmaps(polyCount);
	for(int i=0; i<polyCount; ++i)
	{
		std::string filename = lightmapPrefix + lexical_cast<std::string,int>(i) + ".bmp";
		lightmaps[i] = BitmapLoader::load_image24(filename);
	}

	// Load the onion tree.
	typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;
	ColPolyVector onionPolygons;
	OnionTree_Ptr onionTree;
	OnionTreeFileUtil::load(onionTreeFilename, onionPolygons, onionTree);

	// Load the onion portals.
	std::vector<OnionPortal_Ptr> onionPortals = OnionPortalsFileUtil::load(onionPortalsFilename);

	// Load the navigation data.
	std::vector<NavDataset_Ptr> navDatasets = NavFileUtil::load(navFilename);

	// TODO: Load the entities.

	// Write everything to the output file.
	LevelFileUtil::save_lit(outputFilename,
							polygons, tree,
							portals,
							leafVis,
							lightmaps,
							onionPolygons, onionTree,
							onionPortals,
							navDatasets);
}
catch(Exception& e) { quit_with_error(e.cause()); }

void collate_unlit(const std::string& treeFilename, const std::string& portalsFilename, const std::string& visFilename,
				   const std::string& onionTreeFilename, const std::string& onionPortalsFilename,
				   const std::string& navFilename, const std::string& entitiesFilename, const std::string& outputFilename)
try
{
	// Load the unlit polygons and tree.
	typedef std::vector<TexturedPolygon_Ptr> TexPolyVector;
	TexPolyVector polygons;
	BSPTree_Ptr tree;
	TreeFileUtil::load(treeFilename, polygons, tree);

	// Load the portals.
	int emptyLeafCount;
	std::vector<Portal_Ptr> portals;
	PortalsFileUtil::load(portalsFilename, emptyLeafCount, portals);

	// Load the vis table.
	LeafVisTable_Ptr leafVis = VisFileUtil::load(visFilename);

	// Load the onion tree.
	typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;
	ColPolyVector onionPolygons;
	OnionTree_Ptr onionTree;
	OnionTreeFileUtil::load(onionTreeFilename, onionPolygons, onionTree);

	// Load the onion portals.
	std::vector<OnionPortal_Ptr> onionPortals = OnionPortalsFileUtil::load(onionPortalsFilename);

	// Load the navigation data.
	std::vector<NavDataset_Ptr> navDatasets = NavFileUtil::load(navFilename);

	// TODO: Load the entities.

	// Write everything to the output file.
	LevelFileUtil::save_unlit(outputFilename,
							  polygons, tree,
							  portals,
							  leafVis,
							  onionPolygons, onionTree,
							  onionPortals,
							  navDatasets);
}
catch(Exception& e) { quit_with_error(e.cause()); }

int main(int argc, char *argv[])
{
	if(argc != 10) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);

	if(args[1] == "+L") collate_lit(args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);
	else if(args[1] == "-L") collate_unlit(args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]);
	else quit_with_usage();

	return 0;
}
