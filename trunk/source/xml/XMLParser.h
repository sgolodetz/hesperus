/***
 * hesperus: XMLParser.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_XMLPARSER
#define H_HESP_XMLPARSER

#include "XMLLexer.h"
#include "XMLTree.h"

namespace hesp {

class XMLParser
{
	//#################### PRIVATE VARIABLES ####################
private:
	XMLLexer_Ptr m_lexer;

	//#################### CONSTRUCTORS ####################
public:
	XMLParser(const XMLLexer_Ptr& lexer);

	//#################### PUBLIC METHODS ####################
public:
	XMLTree_CPtr tree() const;
};

}

#endif
