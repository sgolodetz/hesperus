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

	read_line(is, line, "LightmapPrefix");
	read_line(is, line, "{");
	read_line(is, lightmapPrefix, "lightmap prefix");
	read_line(is, line, "}");

	return lightmapPrefix;
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

	read_line(is, line, "VisTable");
	read_line(is, line, "{");

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

	read_line(is, line, "}");

	return leafVis;
}

//#################### LOADING SUPPORT METHODS ####################
/**
Attempts to read a line from a std::istream into a string.

@param is			The std::istream
@param line			The string into which to read
@param expected		A description of what we were expecting the line to contain
@throws Exception	If the read fails
*/
void FileSectionUtil::read_line(std::istream& is, std::string& line, const std::string& expected)
{
	if(!std::getline(is, line)) throw Exception("Unexpected EOF whilst trying to read " + expected);
}

}
