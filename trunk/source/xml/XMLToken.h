/***
 * hesperus: XMLToken.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_XMLTOKEN
#define H_HESP_XMLTOKEN

#include <string>

namespace hesp {

enum TokenType
{
	XMLT_EQUALS,		// =
	XMLT_IDENT,			// identifier
	XMLT_LBRACKET,		// <
	XMLT_LSLASH,		// </
	XMLT_RBRACKET,		// >
	XMLT_RSLASH,		// />
	XMLT_VALUE,			// "attribute value"
};

class XMLToken
{
	//#################### PRIVATE VARIABLES ####################
private:
	TokenType m_type;
	std::string m_value;

	//#################### CONSTRUCTORS ####################
public:
	XMLToken(TokenType type, const std::string& value)
	:	m_type(type), m_value(value)
	{}

	//#################### PUBLIC METHODS ####################
public:
	TokenType type() const				{ return m_type; }
	const std::string& value() const	{ return m_value; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<XMLToken> XMLToken_Ptr;

}

#endif
