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

#include <source/level/onionbsp/OnionCompiler.h>
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
	// TODO
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

	// TODO

	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
