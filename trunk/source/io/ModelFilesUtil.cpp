/***
 * hesperus: ModelFilesUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ModelFilesUtil.h"

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>
#include <source/xml/XMLParser.h>

namespace hesp {

//#################### LOADING SUPPORT METHODS ####################
Skeleton_Ptr ModelFilesUtil::load_skeleton(const std::string& filename)
{
	XMLLexer_Ptr lexer(new XMLLexer(filename));
	XMLParser parser(lexer);
	XMLElement_CPtr root = parser.parse();

	XMLElement_CPtr skeletonElt = root->find_unique_child("skeleton");
	XMLElement_CPtr bonesElt = skeletonElt->find_unique_child("bones");
	XMLElement_CPtr bonehierarchyElt = skeletonElt->find_unique_child("bonehierarchy");

	std::vector<XMLElement_CPtr> boneElts = bonesElt->find_children("bone");
	int boneCount = static_cast<int>(boneElts.size());

	std::vector<Bone_Ptr> bones(boneCount);
	for(int i=0; i<boneCount; ++i)
	{
		const XMLElement_CPtr& boneElt = boneElts[i];
		int id = lexical_cast<int,std::string>(boneElt->attribute("id"));
		std::string name = boneElt->attribute("name");
		// TODO
	}

	// NYI
	throw 23;
}

}
