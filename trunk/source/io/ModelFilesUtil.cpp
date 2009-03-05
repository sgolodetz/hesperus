/***
 * hesperus: ModelFilesUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ModelFilesUtil.h"

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>

namespace hesp {

//#################### LOADING SUPPORT METHODS ####################
Vector3d ModelFilesUtil::extract_vector3d(const XMLElement_CPtr& elt)
{
	double x = lexical_cast<double,std::string>(elt->attribute("x"));
	double y = lexical_cast<double,std::string>(elt->attribute("y"));
	double z = lexical_cast<double,std::string>(elt->attribute("z"));
	return Vector3d(x,y,z);
}

Skeleton_Ptr ModelFilesUtil::load_skeleton(const std::string& filename)
try
{
	XMLLexer_Ptr lexer(new XMLLexer(filename));
	XMLParser parser(lexer);
	XMLElement_CPtr root = parser.parse();
	XMLElement_CPtr skeletonElt = root->find_unique_child("skeleton");

	// Load in the bones.
	XMLElement_CPtr bonesElt = skeletonElt->find_unique_child("bones");

	std::vector<XMLElement_CPtr> boneElts = bonesElt->find_children("bone");
	int boneCount = static_cast<int>(boneElts.size());

	std::vector<Bone_Ptr> bones(boneCount);
	for(int i=0; i<boneCount; ++i)
	{
		// Extract all the necessary bits of information from the XML tree below each bone, then construct the bone itself.
		const XMLElement_CPtr& boneElt = boneElts[i];

		int id = lexical_cast<int,std::string>(boneElt->attribute("id"));

		std::string name = boneElt->attribute("name");

		XMLElement_CPtr positionElt = boneElt->find_unique_child("position");
		Vector3d position = extract_vector3d(positionElt);

		XMLElement_CPtr rotationElt = boneElt->find_unique_child("rotation");

		double rotationAngle = lexical_cast<double,std::string>(rotationElt->attribute("angle"));

		XMLElement_CPtr axisElt = rotationElt->find_unique_child("axis");
		Vector3d rotationAxis = extract_vector3d(axisElt);

		bones[id].reset(new Bone(name, position, rotationAxis, rotationAngle));
	}

	// Construct the bone configuration.
	BoneConfiguration_Ptr boneConfiguration(new BoneConfiguration(bones));

	// Load in the bone hierarchy.
	XMLElement_CPtr bonehierarchyElt = skeletonElt->find_unique_child("bonehierarchy");
	std::vector<XMLElement_CPtr> boneparentElts = bonehierarchyElt->find_children("boneparent");
	int boneparentCount = static_cast<int>(boneparentElts.size());
	for(int i=0; i<boneparentCount; ++i)
	{
		const XMLElement_CPtr& boneparentElt = boneparentElts[i];
		std::string childName = boneparentElt->attribute("bone");
		std::string parentName = boneparentElt->attribute("parent");
		Bone_Ptr child = boneConfiguration->bones(childName);
		Bone_Ptr parent = boneConfiguration->bones(parentName);
		child->set_parent(parent);
	}

	// Load in the animations.
	// TODO

	// NYI
	throw 23;
}
catch(bad_lexical_cast&)
{
	throw Exception("An element attribute was not of the correct type");
}

}
