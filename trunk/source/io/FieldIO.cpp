/***
 * hesperus: FieldIO.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FieldIO.h"

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>

namespace hesp {

//#################### READING METHODS ####################
std::string FieldIO::read_field(std::istream& is, const std::string& expectedFieldName)
{
	std::string field;

	LineIO::read_line(is, field, expectedFieldName);

	size_t i = field.find('=');
	if(i == std::string::npos) throw Exception("Bad field " + expectedFieldName);

	std::string fieldName = field.substr(0, i-1);
	if(fieldName != expectedFieldName) throw Exception("Bad field name " + fieldName + ", expected " + expectedFieldName);

	if(field.length() <= i+2) throw Exception("Missing field value for " + expectedFieldName);
	field = field.substr(i+2);

	return field;
}

std::vector<int> FieldIO::read_intarray_field(std::istream& is, const std::string& expectedFieldName)
{
	std::vector<int> arr;

	std::string fieldString = read_field(is, expectedFieldName);

	typedef boost::char_separator<char> sep;
	typedef boost::tokenizer<sep> tokenizer;

	tokenizer tok(fieldString.begin(), fieldString.end(), sep("[,]"));
	std::vector<std::string> tokens(tok.begin(), tok.end());
	for(size_t i=0, size=tokens.size(); i<size; ++i)
	{
		arr.push_back(lexical_cast<int,std::string>(tokens[i]));
	}

	return arr;
}

}
