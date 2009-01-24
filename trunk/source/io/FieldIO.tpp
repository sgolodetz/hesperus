/***
 * hesperus: FieldIO.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <boost/lexical_cast.hpp>

#include <source/exceptions/Exception.h>

namespace hesp {

//#################### READING METHODS ####################
template <typename T>
T FieldIO::read_typed_field(std::istream& is, const std::string& expectedFieldName)
{
	std::string fieldString = read_field(is, expectedFieldName);
	try
	{
		T field = boost::lexical_cast<T,std::string>(fieldString);
		return field;
	}
	catch(boost::bad_lexical_cast&)
	{
		throw Exception("The value for " + expectedFieldName + " was not of the right type");
	}
}

}
