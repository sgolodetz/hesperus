/***
 * hnav: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <source/exceptions/Exception.h>
#include <source/io/NavFileUtil.h>
#include <source/io/OnionTreeFileUtil.h>
#include <source/level/nav/AdjacencyTable.h>
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
	std::cout << "Usage: hnav <input onion tree> <output navmesh stem>" << std::endl;
	exit(EXIT_FAILURE);
}

void run(const std::string& inputFilename, const std::string& outputStem)
{
	typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;

	// Read in the polygons and onion tree.
	ColPolyVector polygons;
	OnionTree_Ptr tree;
	OnionTreeFileUtil::load(inputFilename, polygons, tree);

	// For each separate map.
	int mapCount = tree->map_count();
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
		NavMeshGenerator generator(mapPolygons);
		NavMesh_Ptr mesh = generator.generate_mesh();

		// Build the navigation graph adjacency list.
		AdjacencyList_Ptr adjList(new AdjacencyList(mesh));

		// Build the navigation graph adjacency table (note that this is a very inefficient
		// representation for the sparse graph in terms of space, but it's needed for the
		// Floyd-Warshall algorithm used when building the path table).
		AdjacencyTable adjTable(*adjList);

		// Generate the path table.
		PathTable_Ptr pathTable = PathTableGenerator::floyd_warshall(adjTable);

		// Write the navigation mesh to disk.
		// TODO: Save the adjacency lists and path tables as well.
		std::string outputFilename = outputStem + "-" + boost::lexical_cast<std::string,int>(i) + ".nav";
		NavFileUtil::save(outputFilename, mesh);
	}
}

int main(int argc, char *argv[])
try
{
	if(argc != 3) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run(args[1], args[2]);
	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
