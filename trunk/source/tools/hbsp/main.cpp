/***
 * hbsp: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/io/GeometryFile.h>
#include <source/io/TreeFile.h>
#include <source/level/trees/BSPCompiler.h>
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
	std::cout << "Usage: hbsp {-r|-c} <input filename> <output filename> [-w<number>]" << std::endl;
	exit(EXIT_FAILURE);
}

template <typename Poly>
void run_compiler(const std::string& inputFilename, const std::string& outputFilename, const double weight)
{
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	// Load the input polygons from disk.
	PolyVector polygons;
	GeometryFile::load(inputFilename, polygons);

	// Build the BSP tree.
	BSPTree_Ptr tree = BSPCompiler::build_tree(polygons, weight);

	// Save the polygons and the BSP tree to the output file.
	TreeFile::save(outputFilename, polygons, tree);
}

int main(int argc, char *argv[])
try
{
	if(argc != 4 && argc != 5) quit_with_usage();

	const std::vector<std::string> args(argv, argv+argc);

	std::string inputFilename = args[2];
	std::string outputFilename = args[3];

	double weight = 4;
	if(argc == 5)
	{
		if(args[4].substr(0,2) != "-w") quit_with_usage();

		try							{ weight = lexical_cast<double,std::string>(args[4].substr(2)); }
		catch(bad_lexical_cast&)	{ quit_with_usage(); }
	}

	if(args[1] == "-r") run_compiler<TexturedPolygon>(inputFilename, outputFilename, weight);
	else if(args[1] == "-c") run_compiler<CollisionPolygon>(inputFilename, outputFilename, weight);
	else quit_with_usage();

	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
