/***
 * hlight: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/images/BitmapSaver.h>
#include <source/io/FileSectionUtil.h>
#include <source/io/FileUtil.h>
#include <source/level/lighting/LightmapGenerator.h>
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
	std::cout << "Usage: hlight <input tree> <input vis> <input lights> <lightmap file prefix> <output filename>" << std::endl;
	exit(EXIT_FAILURE);
}

void run_generator(const std::string& treeFilename, const std::string& visFilename, const std::string& lightsFilename,
				   const std::string& lightmapPrefix, const std::string& outputFilename)
try		// <--- Note the "function try" syntax (this is a rarely-used C++ construct).
{
	// Read in the polygons and tree.
	std::vector<TexturedPolygon_Ptr> polygons;
	BSPTree_Ptr tree;
	FileUtil::load_tree_file(treeFilename, polygons, tree);

	// Read in the vis table.
	LeafVisTable_Ptr leafVis = FileUtil::load_vis_file(visFilename);

	// Read in the lights.
	std::vector<Light> lights = FileUtil::load_lights_file(lightsFilename);

	// Note:	We try and open the output file now because the lightmap generation process
	//			is a potentially lengthy one: it would be very annoying for users if they
	//			spent ages waiting for the calculations to finish and then found that they
	//			couldn't be written to file.
	std::ofstream os(outputFilename.c_str());
	if(os.fail()) quit_with_error("Couldn't open output file for writing");

	// Generate the lit polygons and lightmaps.
	LightmapGenerator lg(polygons, lights, tree, leafVis);
	lg.generate_lightmaps();

	typedef std::vector<Lightmap_Ptr> LightmapVector;
	typedef shared_ptr<LightmapVector> LightmapVector_Ptr;
	typedef std::vector<TexturedLitPolygon_Ptr> TexLitPolyVector;
	typedef shared_ptr<TexLitPolyVector> TexLitPolyVector_Ptr;

	TexLitPolyVector_Ptr litPolygons = lg.lit_polygons();
	LightmapVector_Ptr lightmaps = lg.lightmaps();

	// Write the lit polygons, tree and lightmap prefix to the output file.
	write_polygons(os, *litPolygons);
	os << "***\n";
	tree->output_postorder_text(os);
	os << "***\n";
	FileSectionUtil::save_lightmap_prefix_section(os, lightmapPrefix);
	os.close();

	// Write the lightmaps out as 24-bit bitmaps.
	int lightmapCount = static_cast<int>(lightmaps->size());
	for(int i=0; i<lightmapCount; ++i)
	{
		std::string lightmapFilename = lightmapPrefix + lexical_cast<std::string,int>(i) + ".bmp";
		Image24_Ptr image = (*lightmaps)[i]->to_image();
		BitmapSaver::save_image24(lightmapFilename, image);
	}
}
catch(Exception& e) { quit_with_error(e.cause()); }

int main(int argc, char *argv[])
{
	if(argc != 6) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_generator(args[1], args[2], args[3], args[4], args[5]);
	return 0;
}
