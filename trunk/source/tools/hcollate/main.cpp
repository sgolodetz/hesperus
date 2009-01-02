/***
 * hcollate: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <source/exceptions/Exception.h>
#include <source/images/BitmapLoader.h>
#include <source/io/FileUtil.h>
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
	std::cout << "Usage: hcollate <input lit tree> <input portals> <input vis> <output filename>" << std::endl;
	exit(EXIT_FAILURE);
}

void collate(const std::string& treeFilename, const std::string& portalsFilename, const std::string& visFilename, const std::string& outputFilename)
try
{
	// Load the lit polygons, tree and lightmap prefix.
	typedef std::vector<TexturedLitPolygon_Ptr> TexLitPolyVector;
	TexLitPolyVector polygons;
	BSPTree_Ptr tree;
	std::string lightmapPrefix;
	FileUtil::load_lit_tree_file(treeFilename, polygons, tree, lightmapPrefix);

	// Load the portals.
	int emptyLeafCount;
	std::vector<Portal_Ptr> portals;
	FileUtil::load_portals_file(portalsFilename, emptyLeafCount, portals);

	// Load the vis table.
	LeafVisTable_Ptr leafVis = FileUtil::load_vis_file(visFilename);

	// Load the lightmaps.
	int polyCount = static_cast<int>(polygons.size());
	std::vector<Image24_Ptr> lightmaps(polyCount);
	for(int i=0; i<polyCount; ++i)
	{
		std::string filename = lightmapPrefix + lexical_cast<std::string,int>(i) + ".bmp";
		lightmaps[i] = BitmapLoader::load_image24(filename);
	}

	// Write everything to the output file.
	FileUtil::save_level_file(outputFilename, polygons, tree, portals, leafVis, lightmaps);
}
catch(Exception& e) { quit_with_error(e.cause()); }

int main(int argc, char *argv[])
{
	if(argc != 5) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	collate(args[1], args[2], args[3], args[4]);
	return 0;
}
