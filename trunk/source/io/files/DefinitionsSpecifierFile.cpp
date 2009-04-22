/***
 * hesperus: DefinitionsSpecifierFile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "DefinitionsSpecifierFile.h"

#include <fstream>

#include <source/exceptions/Exception.h>
#include <source/io/sections/DefinitionsSpecifierSection.h>

namespace hesp {

//#################### LOADING METHODS ####################
std::string DefinitionsSpecifierFile::load(const std::string& filename)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");
	return DefinitionsSpecifierSection::load(is);
}

//#################### SAVING METHODS ####################
void DefinitionsSpecifierFile::save(const std::string& filename, const std::string& definitionsFilename)
{
	std::ofstream os(filename.c_str());
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");
	DefinitionsSpecifierSection::save(os, definitionsFilename);
}

}
