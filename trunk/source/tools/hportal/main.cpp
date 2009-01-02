/***
 * hportal: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/io/FileUtil.h>
#include <source/level/bsp/BSPTree.h>
#include <source/level/portals/PortalGenerator.h>
#include <source/math/geom/GeomUtil.h>
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
	std::cout << "Usage: hportal {-r|-c} <input filename> <output filename>" << std::endl;
	exit(EXIT_FAILURE);
}

template <typename Poly>
void run_generator(const std::string& inputFilename, const std::string& outputFilename)
{
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	// Read in the polygons and tree.
	PolyVector polygons;
	BSPTree_Ptr tree;
	try
	{
		FileUtil::load_tree_file(inputFilename, polygons, tree);
	}
	catch(Exception& e) { quit_with_error(e.cause()); }

	// Generate the portals.
	shared_ptr<std::list<Portal_Ptr> > portals = PortalGenerator::generate_portals(polygons, tree);

	// Save the portals to the output file.
	std::ofstream os(outputFilename.c_str());
	if(os.fail()) quit_with_error("Could not open output file for writing");

	os << tree->empty_leaf_count() << '\n';

	std::vector<Portal_Ptr> vec(portals->begin(), portals->end());
	write_polygons(os, vec);
}

int main(int argc, char *argv[])
{
	if(argc != 4) quit_with_usage();

	const std::vector<std::string> args(argv, argv+argc);

	std::string inputFilename = args[2];
	std::string outputFilename = args[3];

	if(args[1] == "-r") run_generator<TexturedPolygon>(inputFilename, outputFilename);
	else if(args[1] == "-c") run_generator<CollisionPolygon>(inputFilename, outputFilename);
	else quit_with_usage();

	return 0;
}
