/***
 * hoportal: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/io/OnionTreeFileUtil.h>
#include <source/level/onionbsp/OnionTree.h>
#include <source/level/portals/OnionPortalGenerator.h>
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
	std::cout << "Usage: hoportal {-r|-c} <input filename> <output filename>" << std::endl;
	exit(EXIT_FAILURE);
}

template <typename Poly>
void run_generator(const std::string& inputFilename, const std::string& outputFilename)
{
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	// Read in the polygons and onion tree.
	PolyVector polygons;
	OnionTree_Ptr tree;
	try
	{
		OnionTreeFileUtil::load(inputFilename, polygons, tree);
	}
	catch(Exception& e) { quit_with_error(e.cause()); }

	// Generate the onion portals.
	shared_ptr<std::list<OnionPortal_Ptr> > portals = OnionPortalGenerator().generate_portals(polygons, tree);

	// Save the onion portals to the output file.
	std::ofstream os(outputFilename.c_str());
	if(os.fail()) quit_with_error("Could not open output file for writing");

	std::vector<OnionPortal_Ptr> vec(portals->begin(), portals->end());
	FileSectionUtil::save_polygons_section(os, "OnionPortals", vec);
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
