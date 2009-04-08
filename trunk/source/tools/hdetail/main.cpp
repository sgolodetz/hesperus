/***
 * hdetail: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <source/exceptions/Exception.h>
#include <source/io/GeometryFile.h>
#include <source/io/TreeFile.h>
#include <source/util/PolygonTypes.h>
using namespace hesp;

//#################### TYPEDEFS ####################
typedef std::vector<TexturedPolygon_Ptr> TexPolyVector;

//#################### FUNCTIONS ####################
void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: hdetail <input BSP tree> <input detail geometry> <output BSP tree>" << std::endl;
	exit(EXIT_FAILURE);
}

void run_detailer(const std::string& inputBSPFilename, const std::string& inputDetailGeometryFilename,
				  const std::string& outputBSPFilename)
{
	// Read in the polygons and tree.
	TexPolyVector polygons;
	BSPTree_Ptr tree;
	TreeFile::load(inputBSPFilename, polygons, tree);

	// Read in the detail geometry.
	TexPolyVector detailPolygons;
	GeometryFile::load(inputDetailGeometryFilename, detailPolygons);

	// Clip each detail face to the tree and add face fragments to the relevant leaves.
	// NYI
	throw 23;

	// Write the modified polygon array and tree to disk.
	TreeFile::save(outputBSPFilename, polygons, tree);
}

int main(int argc, char *argv[])
try
{
	if(argc != 4) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_detailer(args[1], args[2], args[3]);
	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
