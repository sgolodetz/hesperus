/***
 * hexpand: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/io/BrushesFileUtil.h>
#include <source/level/brushes/BrushExpander.h>
#include <source/math/geom/AABB.h>
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
	std::cout << "Usage: hexpand <input AABBs> <input brushes>" << std::endl;
	exit(EXIT_FAILURE);
}

std::vector<AABB3d> load_aabbs_file(const std::string& filename)
{
	std::vector<AABB3d> aabbs;

	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	// Read the AABB count.
	std::string line;
	if(!std::getline(is,line)) throw Exception("Unexpected EOF whilst trying to read AABB count");

	int aabbCount;
	try							{ aabbCount = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The AABB count was not an integer"); }

	// Read the AABBs.
	for(int i=0; i<aabbCount; ++i)
	{
		if(!std::getline(is,line)) throw Exception("Unexpected EOF whilst trying to read AABB");
		AABB3d aabb = read_aabb<Vector3d>(line);
		aabbs.push_back(aabb);
	}

	return aabbs;
}

void run_expander(const std::string& aabbsFilename, const std::string& inputFilename)
{
	// Read in the input AABBs.
	std::vector<AABB3d> aabbs = load_aabbs_file(aabbsFilename);

	// Read in the input brushes.
	typedef PolyhedralBrush<CollisionPolygon> ColPolyBrush;
	typedef shared_ptr<ColPolyBrush> ColPolyBrush_Ptr;
	typedef std::vector<ColPolyBrush_Ptr> ColPolyBrushVector;
	ColPolyBrushVector inputBrushes = BrushesFileUtil::load<CollisionPolygon>(inputFilename);

	// Calculate the output stem and extension.
	std::string::size_type k = inputFilename.find('.');
	std::string outputStem;
	if(k == std::string::npos) outputStem = inputFilename + "-";
	else outputStem = inputFilename.substr(0,k) + "-";

	const std::string outputExtension = ".ebr";

	// For each AABB, expand the brushes and write the expanded brushes to file.
	int aabbCount = static_cast<int>(aabbs.size());
	int brushCount = static_cast<int>(inputBrushes.size());
	for(int i=0; i<aabbCount; ++i)
	{
		// Expand the brushes.
		ColPolyBrushVector expandedBrushes(brushCount);
		for(int j=0; j<brushCount; ++j)
		{
			expandedBrushes[j] = BrushExpander::expand_brush(inputBrushes[j], aabbs[i]);
		}

		// Write the expanded brushes to file.
		std::ostringstream oss;
		oss << outputStem << i << outputExtension;
		BrushesFileUtil::save(oss.str(), expandedBrushes);
	}
}

int main(int argc, char *argv[])
try
{
	if(argc != 3) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_expander(args[1], args[2]);
	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
