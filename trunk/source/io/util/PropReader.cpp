/***
 * hesperus: PropReader.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PropReader.h"

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

namespace hesp {

//#################### PropReaderUtil - PUBLIC METHODS ####################
std::string PropReaderUtil::next_sized_token(std::string& input)
{
	size_t i = input.find_first_of(' ');
	if(i == std::string::npos) throw bad_lexical_cast();
	std::string lenString = input.substr(1,i-2);
	int len = lexical_cast<int,std::string>(lenString);
	std::string tok = input.substr(i+1,len);
	input.erase(0,i+len+2);
	return tok;
}

std::string PropReaderUtil::next_token(std::string& input)
{
	size_t i = input.find_first_of(' ');
	std::string tok = input.substr(0,i);
	input.erase(0, tok.length()+1);
	return tok;
}

//#################### SPECIALIZATIONS ####################
ObjectID PropReader<ObjectID>::read(std::string& input)
{
	return ObjectID(PropReader<int>::read(input));
}

std::string PropReader<std::string>::read(std::string& input)
{
	return PropReaderUtil::next_sized_token(input);
}

Vector3d PropReader<Vector3d>::read(std::string& input)
{
	std::string tok = PropReaderUtil::next_sized_token(input);
	double x = PropReader<double>::read(tok);
	double y = PropReader<double>::read(tok);
	double z = PropReader<double>::read(tok);
	return Vector3d(x,y,z);
}

}
