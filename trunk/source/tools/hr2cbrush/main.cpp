/***
 * hr2cbrush: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/io/FileUtil.h>
#include <source/level/csg/PolyhedralBrush.h>
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
	std::cout << "Usage: hr2cbrush <input filename> <output filename>" << std::endl;
	exit(EXIT_FAILURE);
}

void run_converter(const std::string& inputFilename, const std::string& outputFilename)
{
	// Read in the rendering brushes.
	typedef TexturedPolygon::Vert TexVert;
	typedef TexturedPolygon::AuxData TexAuxData;
	typedef PolyhedralBrush<TexVert,TexAuxData> TexPolyBrush;
	typedef shared_ptr<TexPolyBrush> TexPolyBrush_Ptr;
	typedef std::vector<TexPolyBrush_Ptr> TexPolyBrushVector;
	TexPolyBrushVector brushes = FileUtil::load_brushes_file<TexVert,TexAuxData>(inputFilename);

	// Convert each rendering brush into a collision brush.
	// TODO
}

int main(int argc, char *argv[])
{
	if(argc != 3) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_converter(args[1], args[2]);
	return 0;
}
