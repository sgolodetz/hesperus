/***
 * hesperus: FileUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FileUtil.h"

#include <fstream>
#include <sstream>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>

namespace hesp {

/**
Loads an array of lights from the specified file.

@param filename	The name of the file containing the list of lights
@return			The array of lights
*/
std::vector<Light> load_lights_file(const std::string& filename)
{
	std::vector<Light> lights;

	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("The lights file could not be read");

	// Read in the light count.
	int lightCount;

	std::string line;
	try
	{
		std::getline(is, line);
		lightCount = lexical_cast<int,std::string>(line);
		lights.reserve(lightCount);
	}
	catch(bad_lexical_cast&) { throw Exception("The light count was not an integer"); }

	// Read in the lights, one per line.
	for(int i=0; i<lightCount; ++i)
	{
		if(!std::getline(is, line))
		{
			std::ostringstream oss;
			oss << "Unexpected EOF at line " << i << " in the lights file";
			throw Exception(oss.str());
		}

		// Parse the line.
		typedef boost::char_separator<char> sep;
		typedef boost::tokenizer<sep> tokenizer;
		tokenizer tok(line.begin(), line.end(), sep(" "));
		std::vector<std::string> tokens(tok.begin(), tok.end());

		if(tokens.size() != 10) throw Exception("Bad light data at line " + lexical_cast<std::string,int>(i));

		std::vector<std::string> positionComponents(&tokens[1], &tokens[4]);
		std::vector<std::string> colourComponents(&tokens[6], &tokens[9]);

		Vector3d position(positionComponents);
		Colour3d colour(colourComponents);
		lights.push_back(Light(position, colour));
	}

	return lights;
}

/**
Loads a leaf visibility table from the specified file.

@param filename	The name of the file from which to load the visibility table
@return			The visibility table
*/
LeafVisTable_Ptr load_vis_file(const std::string& filename)
{
	LeafVisTable_Ptr leafVis;

	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("The vis file could not be read");

	std::string line;

	// Read in the size of the vis table.
	int size;
	if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read vis table size");
	try							{ size = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The vis table size was not an integer"); }

	// Construct an empty vis table of the right size.
	leafVis.reset(new LeafVisTable(size));

	// Read in the vis table itself.
	for(int i=0; i<size; ++i)
	{
		if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read vis table row " + lexical_cast<std::string,int>(i));
		if(line.length() != size) throw Exception("Bad vis table row " + lexical_cast<std::string,int>(i));

		for(int j=0; j<size; ++j)
		{
			if(line[j] == '0') (*leafVis)(i,j) = LEAFVIS_NO;
			else if(line[j] == '1') (*leafVis)(i,j) = LEAFVIS_YES;
			else throw Exception("Bad vis table value in row " + lexical_cast<std::string,int>(i));
		}
	}

	return leafVis;
}

}
