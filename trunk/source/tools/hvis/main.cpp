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

#include <source/io/FileUtil.h>
#include <source/level/vis/VisCalculator.h>
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
try
{
	// Note:	We try and open the output file now because the visibility calculation
	//			process is a potentially lengthy one: it would be very annoying for users
	//			if they spent ages waiting for the calculations to finish and then found
	//			that it couldn't be written to file.
	std::ofstream os(outputFilename.c_str());
	if(os.fail()) quit_with_error("Output file could not be opened for writing");

	// Read in the empty leaf count and portals.
	int emptyLeafCount;
	std::vector<Portal_Ptr> portals;
	FileUtil::load_portals_file(inputFilename, emptyLeafCount, portals);

	// Run the visibility calculator.
	VisCalculator visCalc(emptyLeafCount, portals);
	LeafVisTable_Ptr leafVis = visCalc.calculate_leaf_vis_table();

	// Write the leaf visibility table to the output file.
	FileUtil::save_vis_section(os, leafVis);
}
catch(Exception& e) { quit_with_error(e.cause()); }

int main(int argc, char *argv[])
{
	if(argc != 3) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_calculator(args[1], args[2]);
	return 0;
}
