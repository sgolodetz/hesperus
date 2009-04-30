/***
 * hesperus: DefinitionsFile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "DefinitionsFile.h"

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
Loads the specified definitions file.

@param filename				The name of the file
@param entityAABBs			Used to return the AABBs used for entities
@param entityPropertyTypes	Used to return the types of the various entity properties
*/
void DefinitionsFile::load(const std::string& filename, std::vector<AABB3d>& entityAABBs, std::map<std::string,std::string>& entityPropertyTypes)
{
	XMLLexer_Ptr lexer(new XMLLexer(filename));
	XMLParser parser(lexer);
	XMLElement_CPtr root = parser.parse();
	XMLElement_CPtr definitionsElt = root->find_unique_child("definitions");

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Process the entities subtree.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	{
		XMLElement_CPtr entitiesElt = definitionsElt->find_unique_child("entities");

		// Load the entity AABBs.
		XMLElement_CPtr aabbsElt = entitiesElt->find_unique_child("aabbs");
		std::vector<XMLElement_CPtr> aabbElts = aabbsElt->find_children("aabb");

		for(size_t i=0, size=aabbElts.size(); i<size; ++i)
		{
			const XMLElement_CPtr& aabbElt = aabbElts[i];
			XMLElement_CPtr minsElt = aabbElt->find_unique_child("mins");
			XMLElement_CPtr maxsElt = aabbElt->find_unique_child("maxs");
			entityAABBs.push_back(AABB3d(extract_vector3d(minsElt), extract_vector3d(maxsElt)));
		}

		// Load the entity property types.
		XMLElement_CPtr propertiesElt = entitiesElt->find_unique_child("properties");
		std::vector<XMLElement_CPtr> propertyElts = propertiesElt->find_children("property");

		for(size_t i=0, size=propertyElts.size(); i<size; ++i)
		{
			const XMLElement_CPtr& propertyElt = propertyElts[i];
			const std::string& name = propertyElt->attribute("name");
			const std::string& type = propertyElt->attribute("type");
			entityPropertyTypes.insert(std::make_pair(name, type));
		}
	}

	// TODO: Process the other subtrees when it becomes necessary.
}

/**
Loads an array of AABBs from the specified definitions file.

@param filename	The name of the file from which to load the AABBs
@return			The AABBs
*/
std::vector<AABB3d> DefinitionsFile::load_aabbs_only(const std::string& filename)
{
	XMLLexer_Ptr lexer(new XMLLexer(filename));
	XMLParser parser(lexer);
	XMLElement_CPtr root = parser.parse();
	XMLElement_CPtr definitionsElt = root->find_unique_child("definitions");
	XMLElement_CPtr entitiesElt = definitionsElt->find_unique_child("entities");

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
Vector3d DefinitionsFile::extract_vector3d(const XMLElement_CPtr& elt)
{
	// FIXME: This is the same as the method in ModelFiles. Both should be extracted into a single function elsewhere.
	double x = lexical_cast<double,std::string>(elt->attribute("x"));
	double y = lexical_cast<double,std::string>(elt->attribute("y"));
	double z = lexical_cast<double,std::string>(elt->attribute("z"));
	return Vector3d(x,y,z);
}

}
