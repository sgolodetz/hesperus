/***
 * hbsp: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <source/bsp/BSPCompiler.h>
#include <source/math/geom/GeomUtil.h>
#include <source/math/geom/Polygon.h>
#include <source/math/vectors/RenderingVector3d.h>
#include <source/math/vectors/Vector3.h>
#include "QuitFunctions.h"
using namespace hesp;

//#################### ENUMERATIONS ####################
enum GeomType
{
	COLLISION,
	RENDERING
};

//#################### FUNCTIONS ####################
template <typename Vert, typename AuxData>
void run_compiler(const std::string& inputFilename, const std::string& outputFilename, const double weight)
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	// Load the input polygons from disk.
	std::ifstream is(inputFilename.c_str());
	if(is.fail()) quit_with_error("Input file does not exist");
	PolyVector polygons;
	try					{ load_polygons(is, polygons); }
	catch(Exception& e)	{ quit_with_error(e.cause()); }

	// Build the BSP tree.
	BSPTree_Ptr tree = BSPCompiler::build_tree(polygons, weight);

	// Save the polygons and the BSP tree to the output file.
	std::ofstream os(outputFilename.c_str());
	if(os.fail()) quit_with_error("Couldn't open output file for writing");
	write_polygons(os, polygons);
	os << "***\n";
	tree->output_postorder_text(os);
}

int main(int argc, char *argv[])
{
	if(argc != 4 && argc != 5) quit_with_usage();

	const std::vector<std::string> args(argv, argv+argc);

	GeomType geomType;
	if(args[1] == "-r") geomType = RENDERING;
	else if(args[1] == "-c") geomType = COLLISION;
	else quit_with_usage();

	std::string inputFilename = args[2];
	std::string outputFilename = args[3];

	double weight = 4;
	if(argc == 5)
	{
		if(args[4].substr(0,2) != "-w") quit_with_usage();

		try							{ weight = lexical_cast<double,std::string>(args[4].substr(2)); }
		catch(bad_lexical_cast&)	{ quit_with_usage(); }
	}

	switch(geomType)
	{
		case COLLISION:
		{
			run_compiler<Vector3d,bool>(inputFilename, outputFilename, weight);
			break;
		}
		case RENDERING:
		{
			run_compiler<RenderingVector3d,std::string>(inputFilename, outputFilename, weight);
			break;
		}
	}

	return 0;
}
