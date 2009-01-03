/***
 * mef2input: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
	skip_section(is);
}

void read_architecture_brush_composite(std::istream& is)
{
	std::string line;
	if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read ArchitectureBrushComposite");
	if(line != "{") throw Exception("ArchitectureBrushComposite: Expected {");

	for(;;)
	{
		if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read ArchitectureBrushComposite");
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

	if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read MEF ID");
	if(line != "MEF 2") throw Exception("Bad MEF ID or unexpected file version");

	if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read Textures section");
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
