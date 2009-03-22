/***
 * hesperus: FieldIO.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_FIELDIO
#define H_HESP_FIELDIO

#include <iostream>
#include <string>
#include <vector>

#include "LineIO.h"

namespace hesp {

struct FieldIO
{
	//#################### READING METHODS ####################
	static std::string read_field(std::istream& is, const std::string& expectedFieldName);
	static std::vector<int> read_intarray_field(std::istream& is, const std::string& expectedFieldName);
	template <typename T> static T read_typed_field(std::istream& is, const std::string& expectedFieldName);

	//#################### WRITING METHODS ####################
	static void write_intarray_field(std::ostream& os, const std::string& fieldName, const std::vector<int>& arr);
	template <typename T> static void write_typed_field(std::ostream& os, const std::string& fieldName, const T& value);
};

}

#include "FieldIO.tpp"

#endif
