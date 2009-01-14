/***
 * hnav: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <source/exceptions/Exception.h>
#include <source/io/OnionTreeFileUtil.h>
#include <source/level/nav/NavMeshGenerator.h>
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
	std::cout << "Usage: hnav <input onion tree> <output navmesh>" << std::endl;
	exit(EXIT_FAILURE);
}

void run_generator(const std::string& inputFilename, const std::string& outputFilename)
{
	typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;

	// Read in the polygons and onion tree.
	ColPolyVector polygons;
	OnionTree_Ptr tree;
	OnionTreeFileUtil::load(inputFilename, polygons, tree);

	// Generate the navigation mesh.
	NavMeshGenerator generator(polygons, tree);
	// TODO

	// Write the navigation mesh to disk.
	// TODO
}

int main(int argc, char *argv[])
try
{
	if(argc != 3) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_generator(args[1], args[2]);
	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
