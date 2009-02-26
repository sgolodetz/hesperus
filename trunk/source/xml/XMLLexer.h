/***
 * hesperus: XMLLexer.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_XMLLEXER
#define H_HESP_XMLLEXER

#include <istream>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "XMLToken.h"

namespace hesp {

class XMLLexer
{
	//#################### CONSTRUCTORS ####################
public:
	XMLLexer(std::istream& is);

	//#################### PUBLIC METHODS ####################
public:
	XMLToken next_token();
};

//#################### TYPEDEFS ####################
typedef shared_ptr<XMLLexer> XMLLexer_Ptr;

}

#endif
