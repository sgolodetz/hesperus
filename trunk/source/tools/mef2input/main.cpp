/***
 * mef2input: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>
using namespace hesp;

//#################### FUNCTIONS ####################
void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: mef2input <input MEF> <output brushes> <output entities> <output lights>" << std::endl;
	exit(EXIT_FAILURE);
}

void read_line(std::istream& is, std::string& line, const std::string& attemptedAction)
{
	if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to " + attemptedAction);
}

void skip_section(std::istream& is)
{
	std::string line;

	int bracketCount = 0;
	do
	{
		if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to skip section");
		if(line == "{") ++bracketCount;
		if(line == "}") --bracketCount;
	} while(bracketCount > 0);
}

void read_polyhedral_brush(std::istream& is)
{
	std::string line;

	read_line(is, line, "read PolyhedralBrush");
	if(line != "{") throw Exception("PolyhedralBrush: Expected {");

	read_line(is, line, "read bounds");
	if(line.substr(0,6) != "Bounds") throw Exception("PolyhedralBrush: Expected Bounds");

	read_line(is, line, "read polygon count");
	if(line.substr(0,9) != "PolyCount" || line.length() < 11) throw Exception("PolyhedralBrush: Expected PolyCount");

	int polyCount;
	try							{ polyCount = lexical_cast<int,std::string>(line.substr(10)); }
	catch(bad_lexical_cast&)	{ throw Exception("PolyhedralBrush: Polygon count is not an integer"); }

	for(int i=0; i<polyCount; ++i)
	{
		read_line(is, line, "read polygon");
		if(line.substr(0,7) != "Polygon" || line.length() < 9) throw Exception("PolyhedralBrush: Expected Polygon");

		// TODO: Parse polygon.
	}

	read_line(is, line, "read PolyhedralBrush");
	if(line != "}") throw Exception("PolyhedralBrush: Expected }");
}

void read_architecture_brush_composite(std::istream& is)
{
	std::string line;
	read_line(is, line, "read ArchitectureBrushComposite");
	if(line != "{") throw Exception("ArchitectureBrushComposite: Expected {");

	for(;;)
	{
		read_line(is, line, "read ArchitectureBrushComposite");
		if(line == "}") break;

		if(line == "ArchitectureBrushComposite") read_architecture_brush_composite(is);
		else if(line == "PolyhedralBrush") read_polyhedral_brush(is);
		else
		{
			std::cout << "Warning: Don't know how to read a " << line << " subsection of ArchitectureBrushComposite" << std::endl;
			skip_section(is);
		}
	}
}

void run_converter(const std::string& inputFilename, const std::string& brushesFilename, const std::string& entitiesFilename, const std::string& lightsFilename)
{
	std::ifstream is(inputFilename.c_str());
	if(is.fail()) throw Exception("Could not open " + inputFilename + " for reading");

	// Read in the MEF file.
	std::string line;

	read_line(is, line, "read MEF ID");
	if(line != "MEF 2") throw Exception("Bad MEF ID or unexpected file version");

	read_line(is, line, "read Textures");
	if(line != "Textures") throw Exception("Textures section is missing");
	skip_section(is);

	while(std::getline(is, line))
	{
		if(line == "ArchitectureBrushComposite") read_architecture_brush_composite(is);
		else if(line == "PolyhedralBrush") read_polyhedral_brush(is);
		else
		{
			std::cout << "Warning: Don't know how to read a " << line << " section" << std::endl;
			skip_section(is);
		}
	}

	// Write the brushes to disk.
	// TODO

	// Write the entities to disk.
	// TODO

	// Write the lights to disk.
	// TODO
}

int main(int argc, char *argv[])
try
{
	if(argc != 5) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_converter(args[1], args[2], args[3], args[4]);
	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
