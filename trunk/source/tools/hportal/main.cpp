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

#include <source/bsp/BSPTree.h>
#include <source/bsp/PortalGenerator.h>
#include <source/math/geom/GeomUtil.h>
#include <source/math/vectors/RenderingVector3d.h>
#include <source/math/vectors/Vector3.h>
using namespace hesp;

//#################### ENUMERATIONS ####################
enum GeomType
{
	COLLISION,
	RENDERING
};

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

template <typename Vert, typename AuxData>
void run_generator(const std::string& inputFilename, const std::string& outputFilename)
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	std::ifstream is(inputFilename.c_str());
	if(is.fail()) quit_with_error("Input file does not exist");

	std::string line;

	// Read the input polygons.
	PolyVector polygons;
	try
	{
			std::getline(is, line);
			int polyCount = lexical_cast<int,std::string>(line);
			load_polygons(is, polygons, polyCount);
	}
	catch(bad_lexical_cast&)	{ quit_with_error("The polygon count is not an integer"); }
	catch(Exception& e)			{ quit_with_error(e.cause()); }

	// Read the separator.
	std::getline(is, line);
	if(line != "***") throw Exception("Bad separator between the polygons and BSP sections");

	// Read the BSP tree.
	BSPTree_Ptr tree;
	try
	{
		tree = BSPTree::load_postorder_text(is);
	}
	catch(Exception& e) { quit_with_error(e.cause()); }

	// Generate the portals.
	shared_ptr<std::list<Portal_Ptr> > portals = PortalGenerator::generate_portals(polygons, tree);

	// Save the portals to the output file.
	std::ofstream os(outputFilename.c_str());
	if(os.fail()) quit_with_error("Could not open output file for writing");
	std::vector<Portal_Ptr> vec(portals->begin(), portals->end());
	write_polygons(os, vec);
}

int main(int argc, char *argv[])
{
	if(argc != 4) quit_with_usage();

	const std::vector<std::string> args(argv, argv+argc);

	GeomType geomType;
	if(args[1] == "-r") geomType = RENDERING;
	else if(args[1] == "-c") geomType = COLLISION;
	else quit_with_usage();

	std::string inputFilename = args[2];
	std::string outputFilename = args[3];

	switch(geomType)
	{
		case COLLISION:
		{
			run_generator<Vector3d,bool>(inputFilename, outputFilename);
			break;
		}
		case RENDERING:
		{
			run_generator<RenderingVector3d,std::string>(inputFilename, outputFilename);
			break;
		}
	}

	return 0;
}
