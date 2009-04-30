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
	return parse_field(field, expectedFieldName).second;
}

#if 0
std::vector<int> FieldIO::read_intarray_field(std::istream& is, const std::string& expectedFieldName)
{
	std::vector<int> arr;

	std::string fieldValue = read_field(is, expectedFieldName);

	typedef boost::char_separator<char> sep;
	typedef boost::tokenizer<sep> tokenizer;

	tokenizer tok(fieldValue.begin(), fieldValue.end(), sep("[,]"));
	std::vector<std::string> tokens(tok.begin(), tok.end());
	for(size_t i=0, size=tokens.size(); i<size; ++i)
	{
		arr.push_back(lexical_cast<int,std::string>(tokens[i]));
	}

	return arr;
}
#endif

//#################### WRITING METHODS ####################
void FieldIO::write_intarray_field(std::ostream& os, const std::string& fieldName, const std::vector<int>& arr)
{
	os << fieldName << " = [";
	int size = static_cast<int>(arr.size());
	for(int i=0; i<size; ++i)
	{
		os << arr[i];
		if(i < size-1) os << ',';
	}
	os << "]\n";
}

//#################### HELPER METHODS ####################
std::pair<std::string,std::string> FieldIO::parse_field(const std::string& field, const std::string& expectedName)
{
	size_t i = field.find('=');
	if(i == std::string::npos) throw Exception("Bad field: " + field);

	std::string name = field.substr(0, i-1);
	if(expectedName != "" && name != expectedName) throw Exception("Bad field name " + name + ", expected " + expectedName);

	if(field.length() <= i+2) throw Exception("Missing field value: " + field);
	std::string value = field.substr(i+2);

	return std::make_pair(name, value);
}

}
