/***
 * hesperus: EntDefFile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntDefFile.h"

#include <fstream>

// FIXME: These are only necessary because of extract_vector3d() - they can be removed when it's refactored.
#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>
#include <source/io/util/LineIO.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads an array of AABBs from the specified entity definition file.

@param filename	The name of the file from which to load the AABBs
@return			The AABBs
*/
std::vector<AABB3d> EntDefFile::load_aabbs_only(const std::string& filename)
{
	XMLLexer_Ptr lexer(new XMLLexer(filename));
	XMLParser parser(lexer);
	XMLElement_CPtr root = parser.parse();
	XMLElement_CPtr entitiesElt = root->find_unique_child("entities");

	XMLElement_CPtr aabbsElt = entitiesElt->find_unique_child("aabbs");
	std::vector<XMLElement_CPtr> aabbElts = aabbsElt->find_children("aabb");
	int aabbCount = static_cast<int>(aabbElts.size());

	std::vector<AABB3d> aabbs;
	for(int i=0; i<aabbCount; ++i)
	{
		const XMLElement_CPtr& aabbElt = aabbElts[i];
		XMLElement_CPtr minsElt = aabbElt->find_unique_child("mins");
		XMLElement_CPtr maxsElt = aabbElt->find_unique_child("maxs");
		aabbs.push_back(AABB3d(extract_vector3d(minsElt), extract_vector3d(maxsElt)));
	}
	return aabbs;
}

//#################### LOADING SUPPORT METHODS ####################
Vector3d EntDefFile::extract_vector3d(const XMLElement_CPtr& elt)
{
	// FIXME: This is the same as the method in ModelFiles. Both should be extracted into a single function elsewhere.
	double x = lexical_cast<double,std::string>(elt->attribute("x"));
	double y = lexical_cast<double,std::string>(elt->attribute("y"));
	double z = lexical_cast<double,std::string>(elt->attribute("z"));
	return Vector3d(x,y,z);
}

}
