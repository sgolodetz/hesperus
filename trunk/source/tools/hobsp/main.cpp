/***
 * hobsp: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/io/FileUtil.h>
#include <source/level/onionbsp/OnionCompiler.h>
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
	std::cout << "Usage: hobsp {-r|-c} <input geom 1> ... <input geom n> <output tree> [-w<number>]" << std::endl;
	exit(EXIT_FAILURE);
}

template <typename Poly>
void run_compiler(const std::vector<std::string>& inputFilenames, const std::string& outputFilename, double weight)
{
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	// Read in the input maps.
	size_t mapCount = inputFilenames.size();
	std::vector<PolyVector> maps(mapCount);
	for(size_t i=0; i<mapCount; ++i)
	{
		FileUtil::load_polygons_file(inputFilenames[i], maps[i]);
	}

	// Compile them into an onion tree.
	OnionCompiler<Poly> compiler(maps, weight);
	compiler.build_tree();

	// Write the output polygons and onion tree to disk.
	std::ofstream os(outputFilename.c_str());
	if(os.fail()) quit_with_error("Couldn't open output file for writing");
	write_polygons(os, *compiler.polygons());
	os << "***\n";
	compiler.tree()->output_postorder_text(os);
}

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv, argv + argc);

	// If an optional weight argument has been supplied, parse it and remove it to simplify further processing.
	double weight = 4;
	const std::string& lastArg = args[args.size()-1];
	if(lastArg.length() >= 3 && lastArg.substr(0,2) == "-w")
	{
		try							{ weight = lexical_cast<double,std::string>(lastArg.substr(2)); }
		catch(bad_lexical_cast&)	{ quit_with_usage(); }

		args.pop_back();
	}

	// If we don't have (at a minimum) 'hobsp {-r|-c} <input geom 1> <output tree>', then the command is ill-formed.
	if(args.size() < 4) quit_with_usage();

	std::vector<std::string> inputFilenames(&args[2], &args[args.size()-1]);
	const std::string& outputFilename = args[args.size()-1];

	size_t len = outputFilename.length();
	if(len > 4 && outputFilename.substr(len-4) == ".cg2")
		quit_with_error("The extension .cg2 is disallowed for the output tree filename to help prevent errors - sorry!");

	if(args[1] == "-r") run_compiler<TexturedPolygon>(inputFilenames, outputFilename, weight);
	else if(args[1] == "-c") run_compiler<CollisionPolygon>(inputFilenames, outputFilename, weight);
	else quit_with_usage();

	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
