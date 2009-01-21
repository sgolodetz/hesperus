/***
 * hnav: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <source/exceptions/Exception.h>
#include <source/io/EntDefFileUtil.h>
#include <source/io/NavFileUtil.h>
#include <source/io/OnionTreeFileUtil.h>
#include <source/level/nav/AdjacencyTable.h>
#include <source/level/nav/NavDataset.h>
#include <source/level/nav/NavMeshGenerator.h>
#include <source/level/nav/PathTableGenerator.h>
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
	std::cout << "Usage: hnav <input entity definitions> <input onion tree> <output navmesh stem>" << std::endl;
	exit(EXIT_FAILURE);
}

void run(const std::string& entDefFilename, const std::string& treeFilename, const std::string& outputFilename)
{
	typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;

	// Read in the AABBs.
	std::vector<AABB3d> aabbs = EntDefFileUtil::load_aabbs_only(entDefFilename);

	// Read in the polygons and onion tree.
	ColPolyVector polygons;
	OnionTree_Ptr tree;
	OnionTreeFileUtil::load(treeFilename, polygons, tree);

	// Check that the number of AABBs and the number of maps in the tree match up.
	int aabbCount = static_cast<int>(aabbs.size());
	int mapCount = tree->map_count();
	if(aabbCount != mapCount) throw Exception("There must be exactly one AABB per map in the onion tree");

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
		double maxHeightDifference = (aabbs[i].maximum().z - aabbs[i].minimum().z) / 2;
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
	NavFileUtil::save(outputFilename, datasets);
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
