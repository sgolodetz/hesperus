/***
 * hcollate: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <source/exceptions/Exception.h>
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
	load_lit_tree_file(treeFilename, polygons, tree, lightmapPrefix);

	// Load the portals.
	// NYI
	throw 23;

	// Load the vis table.
	// NYI
	throw 23;

	// Load the lightmaps.
	// NYI
	throw 23;

	// Write everything to the output file.
	// NYI
	throw 23;
}
catch(Exception& e) { quit_with_error(e.cause()); }

int main(int argc, char *argv[])
{
	if(argc != 5) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	collate(args[1], args[2], args[3], args[4]);
	return 0;
}
