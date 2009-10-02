/***
 * hnav: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem/operations.hpp>
namespace bf = boost::filesystem;

#include <source/exceptions/Exception.h>
#include <source/io/files/DefinitionsFile.h>
#include <source/io/files/DefinitionsSpecifierFile.h>
#include <source/io/files/NavFile.h>
#include <source/io/files/OnionTreeFile.h>
#include <source/io/util/DirectoryFinder.h>
#include <source/level/collisions/Bounds.h>
#include <source/level/collisions/BoundsManager.h>
#include <source/level/nav/AdjacencyList.h>
#include <source/level/nav/AdjacencyTable.h>
#include <source/level/nav/NavDataset.h>
#include <source/level/nav/NavMeshGenerator.h>
#include <source/level/nav/PathTableGenerator.h>
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
	std::cout << "Usage: hnav <input definitions specifier> <input onion tree> <output navmesh stem>" << std::endl;
	exit(EXIT_FAILURE);
}

void run(const std::string& definitionsSpecifierFilename, const std::string& treeFilename, const std::string& outputFilename)
{
	typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;

	// Read in the definitions specifier.
	std::string definitionsFilename = DefinitionsSpecifierFile::load(definitionsSpecifierFilename);

	// Read in the bounds.
	bf::path baseDir = determine_base_directory();
	bf::path settingsDir = determine_settings_directory(baseDir);
	BoundsManager_Ptr boundsManager = DefinitionsFile::load_bounds_only((settingsDir / definitionsFilename).file_string());

	// Read in the polygons and onion tree.
	ColPolyVector polygons;
	OnionTree_Ptr tree;
	OnionTreeFile::load(treeFilename, polygons, tree);

	// Check that the number of bounds and the number of maps in the tree match up.
	int boundsCount = boundsManager->bounds_count();
	int mapCount = tree->map_count();
	if(boundsCount != mapCount) throw Exception("There must be exactly one bounds per map in the onion tree");

	std::vector<NavDataset_Ptr> datasets;

	// For each separate map.
	for(int i=0; i<mapCount; ++i)
	{
		// Make a copy of the polygon array in which all the polygons that aren't
		// in this map are set to non-walkable.
		int polyCount = static_cast<int>(polygons.size());
		ColPolyVector mapPolygons(polyCount);
		for(int j=0; j<polyCount; ++j)
		{
			mapPolygons[j].reset(new CollisionPolygon(*polygons[j]));
			if(mapPolygons[j]->auxiliary_data().map_index() != i)
				mapPolygons[j]->auxiliary_data().set_walkable(false);
		}

		// Generate the navigation mesh.
		double maxHeightDifference = boundsManager->bounds(i)->height() / 2;
		NavMeshGenerator generator(mapPolygons, maxHeightDifference);
		NavMesh_Ptr mesh = generator.generate_mesh();

		// Build the navigation graph adjacency list.
		AdjacencyList_Ptr adjList(new AdjacencyList(mesh));

		// Build the navigation graph adjacency table (note that this is a very inefficient
		// representation for the sparse graph in terms of space, but it's needed for the
		// Floyd-Warshall algorithm used when building the path table).
		AdjacencyTable adjTable(*adjList);

		// Generate the path table.
		PathTable_Ptr pathTable = PathTableGenerator::floyd_warshall(adjTable);

		datasets.push_back(NavDataset_Ptr(new NavDataset(adjList, mesh, pathTable)));
	}

	// Write the navigation datasets to disk.
	NavFile::save(outputFilename, datasets);
}

int main(int argc, char *argv[])
try
{
	if(argc != 4) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run(args[1], args[2], args[3]);
	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
