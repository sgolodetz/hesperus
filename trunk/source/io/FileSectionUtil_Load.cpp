/***
 * hesperus: FileSectionUtil_Load.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FileSectionUtil.h"

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads a lightmap prefix from the specified std::istream.

@param is			The std::istream
@return				The lightmap prefix
@throws Exception	If EOF is encountered whilst trying to read the lightmap prefix
*/
std::string FileSectionUtil::load_lightmap_prefix_section(std::istream& is)
{
	std::string line, lightmapPrefix;

	read_checked_line(is, line, "LightmapPrefix");
	read_checked_line(is, line, "{");
	read_line(is, lightmapPrefix, "lightmap prefix");
	read_checked_line(is, line, "}");

	return lightmapPrefix;
}

/**
Loads an array of lights from the specified std::istream.

@param is	The std::istream
@return		The array of lights
*/
std::vector<Light> FileSectionUtil::load_lights_section(std::istream& is)
{
	std::vector<Light> lights;

	std::string line;

	read_checked_line(is, line, "Lights");
	read_checked_line(is, line, "{");

	// Read in the light count.
	int lightCount;
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

	read_checked_line(is, line, "}");

	return lights;
}

/**
Loads a BSP tree from the specified std::istream.

@param is	The std::istream
@return		The BSP tree
*/
BSPTree_Ptr FileSectionUtil::load_tree_section(std::istream& is)
{
	std::string line;
	read_checked_line(is, line, "BSPTree");
	read_checked_line(is, line, "{");
	BSPTree_Ptr tree = BSPTree::load_postorder_text(is);
	read_checked_line(is, line, "}");
	return tree;
}

/**
Loads a leaf visibility table from the specified std::istream.

@param is	The std::istream
@return		The visibility table
*/
LeafVisTable_Ptr FileSectionUtil::load_vis_section(std::istream& is)
{
	LeafVisTable_Ptr leafVis;

	std::string line;

	read_checked_line(is, line, "VisTable");
	read_checked_line(is, line, "{");

	// Read in the size of the vis table.
	read_line(is, line, "vis table size");
	int size;
	try							{ size = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The vis table size was not an integer"); }

	// Construct an empty vis table of the right size.
	leafVis.reset(new LeafVisTable(size));

	// Read in the vis table itself.
	for(int i=0; i<size; ++i)
	{
		read_line(is, line, "vis table row " + lexical_cast<std::string,int>(i));
		if(line.length() != size) throw Exception("Bad vis table row " + lexical_cast<std::string,int>(i));

		for(int j=0; j<size; ++j)
		{
			if(line[j] == '0') (*leafVis)(i,j) = LEAFVIS_NO;
			else if(line[j] == '1') (*leafVis)(i,j) = LEAFVIS_YES;
			else throw Exception("Bad vis table value in row " + lexical_cast<std::string,int>(i));
		}
	}

	read_checked_line(is, line, "}");

	return leafVis;
}

//#################### LOADING SUPPORT METHODS ####################
/**
Attempts to read a line from a std::istream into a string.

@param is			The std::istream
@param line			The string into which to read
@param description	A description of what we were trying to read (in case EOF is encountered)
@throws Exception	If EOF is encountered
*/
void FileSectionUtil::read_line(std::istream& is, std::string& line, const std::string& description)
{
	if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read " + description);
}

/**
Attempts to read a line from a std::istream into a string and check its validity.

@param is			The std::istream
@param line			The string into which to read
@param expected		What we expect the line to be
@throws Exception	If EOF is encountered
*/
void FileSectionUtil::read_checked_line(std::istream& is, std::string& line, const std::string& expected)
{
	read_line(is, line, expected);
	if(line != expected) throw Exception("Expected " + expected);
}

}
