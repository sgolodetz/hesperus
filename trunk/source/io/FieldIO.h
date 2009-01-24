/***
 * hesperus: FieldIO.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_FIELDIO
#define H_HESP_FIELDIO

#include <iostream>
#include <string>
#include <vector>

namespace hesp {

struct FieldIO
{
	//#################### READING METHODS ####################
	static std::string read_field(std::istream& is, const std::string& expectedFieldName);
	static std::vector<int> read_intarray_field(std::istream& is, const std::string& expectedFieldName);
	static void read_line(std::istream& is, std::string& line, const std::string& description);
	template <typename T> static T read_typed_field(std::istream& is, const std::string& expectedFieldName);

	//#################### WRITING METHODS ####################
	// TODO
};

}

#include "FieldIO.tpp"

#endif
