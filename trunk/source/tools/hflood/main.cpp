/***
 * hflood: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <source/io/FileUtil.h>
#include <source/level/bsp/BSPTree.h>
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
	std::cout << "Usage: hflood {-r|-c} <input tree> <input portals> <output geometry>" << std::endl;
	exit(EXIT_FAILURE);
}

bool is_valid_portal(const Portal_Ptr& portal)
{
	const double MAXIMUM = 100000;

	// A portal is invalid if any of its vertices have components that are out of range.
	int vertCount = portal->vertex_count();
	for(int i=0; i<vertCount; ++i)
	{
		const Vector3d& v = portal->vertex(i);
		if(fabs(v.x) > MAXIMUM || fabs(v.y) > MAXIMUM || fabs(v.z) > MAXIMUM) return false;
	}

	return true;
}

std::vector<int> valid_leaves(const std::vector<Portal_Ptr>& portals)
{
	std::vector<int> ret;

	int portalCount = static_cast<int>(portals.size());
	for(int i=0; i<portalCount; ++i)
	{
		if(is_valid_portal(portals[i]))
		{
			// If this portal's valid, then so is the leaf it can see.
			ret.push_back(portals[i]->auxiliary_data().toLeaf);
		}
	}

	return ret;
}

template <typename Poly>
void run_cheap_flood(const std::string& treeFilename, const std::string& portalsFilename, const std::string& outputFilename)
{
	// Load the polygons and tree.
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;
	PolyVector polygons;
	BSPTree_Ptr tree;
	FileUtil::load_tree_file(treeFilename, polygons, tree);

	// Load the portals.
	int emptyLeafCount;
	std::vector<Portal_Ptr> portals;
	FileUtil::load_portals_file(portalsFilename, emptyLeafCount, portals);

	// Figure out which leaves are valid.
	std::vector<int> validLeaves = valid_leaves(portals);

	// Copy all the polygons from them to an array.
	PolyVector validPolygons;
	validPolygons.reserve(polygons.size());
	int validLeafCount = static_cast<int>(validLeaves.size());
	for(int i=0; i<validLeafCount; ++i)
	{
		const BSPLeaf *leaf = tree->leaf(validLeaves[i]);
		const std::vector<int>& polyIndices = leaf->polygon_indices();
		for(std::vector<int>::const_iterator jt=polyIndices.begin(), jend=polyIndices.end(); jt!=jend; ++jt)
		{
			validPolygons.push_back(polygons[*jt]);
		}
	}

	// Write the polygons to the output file.
	std::ofstream os(outputFilename.c_str());
	if(os.fail()) throw Exception("Could not open " + outputFilename + " for reading");
	write_polygons(os, validPolygons, false);
}

int main(int argc, char *argv[])
try
{
	if(argc != 5) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);

	if(args[1] == "-r") run_cheap_flood<TexturedPolygon>(args[2], args[3], args[4]);
	else if(args[1] == "-c") run_cheap_flood<CollisionPolygon>(args[2], args[3], args[4]);
	else quit_with_usage();

	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
