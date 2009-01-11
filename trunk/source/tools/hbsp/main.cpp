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

#include <source/io/FileSectionUtil.h>
#include <source/level/bsp/BSPCompiler.h>
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
	std::cout << "Usage: hbsp {-r|-c} <input filename> <output filename> [-w<number>]" << std::endl;
	exit(EXIT_FAILURE);
}

template <typename Poly>
void run_compiler(const std::string& inputFilename, const std::string& outputFilename, const double weight)
{
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	// Load the input polygons from disk.
	std::ifstream is(inputFilename.c_str());
	if(is.fail()) quit_with_error("Input file does not exist");
	PolyVector polygons;
	try					{ FileSectionUtil::load_uncounted_polygons(is, polygons); }
	catch(Exception& e)	{ quit_with_error(e.cause()); }
	is.close();

	// Build the BSP tree.
	BSPTree_Ptr tree = BSPCompiler::build_tree(polygons, weight);

	// Save the polygons and the BSP tree to the output file.
	std::ofstream os(outputFilename.c_str());
	if(os.fail()) quit_with_error("Couldn't open output file for writing");
	FileSectionUtil::save_polygons_section(os, "Polygons", polygons);
	os << "***\n";
	FileSectionUtil::save_tree_section(os, tree);
}

int main(int argc, char *argv[])
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
