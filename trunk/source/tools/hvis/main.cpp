/***
 * hvis: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/bsp/VisCalculator.h>
#include <source/math/geom/GeomUtil.h>
using namespace hesp;

//#################### FUNCTIONS ####################
void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: hvis <input filename> <output filename>" << std::endl;
	exit(EXIT_FAILURE);
}

void run_calculator(const std::string& inputFilename, const std::string& outputFilename)
{
	// Read the input portals.
	std::ifstream is(inputFilename.c_str());
	if(is.fail()) quit_with_error("Input file does not exist");

	std::vector<Portal_Ptr> portals;
	try
	{
		std::string line;
		std::getline(is, line);
		int portalCount = lexical_cast<int,std::string>(line);
		load_polygons(is, portals, portalCount);
	}
	catch(bad_lexical_cast&)	{ quit_with_error("The portal count is not an integer"); }
	catch(Exception& e)			{ quit_with_error(e.cause()); }

	is.close();

	// Run the visibility calculator.
	VisCalculator visCalc(portals);
	VisCalculator::LeafVisTable_Ptr leafVis = visCalc.calculate_leaf_vis_table();

	// Write the leaf visibility table to the output file.
	// TODO
}

int main(int argc, char *argv[])
{
	if(argc != 3) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_calculator(args[1], args[2]);
	return 0;
}
