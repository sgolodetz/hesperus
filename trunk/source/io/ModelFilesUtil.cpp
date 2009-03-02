/***
 * hesperus: ModelFilesUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ModelFilesUtil.h"

#include <source/xml/XMLParser.h>

namespace hesp {

//#################### LOADING SUPPORT METHODS ####################
Skeleton_Ptr ModelFilesUtil::load_skeleton(const std::string& filename)
{
	XMLLexer_Ptr lexer(new XMLLexer(filename));
	XMLParser parser(lexer);
	XMLElement_CPtr root = parser.parse();

	// NYI
	throw 23;
}

}
