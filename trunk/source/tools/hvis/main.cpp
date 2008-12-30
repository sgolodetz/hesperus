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

#include <source/level/vis/VisCalculator.h>
#include <source/math/geom/GeomUtil.h>
using namespace hesp;

//#################### FUNCTIONS ####################
void output_vis_table(std::ostream& os, const LeafVisTable_Ptr& leafVis)
{
	const LeafVisTable& table = *leafVis;

	int size = table.size();
	for(int i=0; i<size; ++i)
	{
		for(int j=0; j<size; ++j)
		{
			switch(table(i,j))
			{
				case LEAFVIS_NO:
				{
					os << '0';
					break;
				}
				case LEAFVIS_YES:
				{
					os << '1';
					break;
				}
			}
		}
		os << '\n';
	}
}

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
	std::ifstream is(inputFilename.c_str());
	if(is.fail()) quit_with_error("Input file does not exist");

	// Note:	We try and open the output file now because the visibility calculation
	//			process is a potentially lengthy one: it would be very annoying for users
	//			if they spent ages waiting for the calculations to finish and then found
	//			that it couldn't be written to file.
	std::ofstream os(outputFilename.c_str());
	if(os.fail()) quit_with_error("Output file could not be opened for writing");

	// Read the empty leaf count.
	std::string line;
	int emptyLeafCount;
	try
	{
		std::getline(is, line);
		emptyLeafCount = lexical_cast<int,std::string>(line);
	}
	catch(bad_lexical_cast&)	{ quit_with_error("The empty leaf count is not an integer"); }

	// Read the input portals.
	std::vector<Portal_Ptr> portals;
	try
	{
		std::getline(is, line);
		int portalCount = lexical_cast<int,std::string>(line);
		load_polygons(is, portals, portalCount);
	}
	catch(bad_lexical_cast&)	{ quit_with_error("The portal count is not an integer"); }
	catch(Exception& e)			{ quit_with_error(e.cause()); }

	is.close();

	// Run the visibility calculator.
	VisCalculator visCalc(emptyLeafCount, portals);
	LeafVisTable_Ptr leafVis = visCalc.calculate_leaf_vis_table();

	// Write the leaf visibility table to the output file.
	output_vis_table(os, leafVis);
}

int main(int argc, char *argv[])
{
	if(argc != 3) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_calculator(args[1], args[2]);
	return 0;
}
