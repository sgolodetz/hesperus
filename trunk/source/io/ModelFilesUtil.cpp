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
	XMLElement_CPtr animationsElt = skeletonElt->find_unique_child("animations");
	std::vector<XMLElement_CPtr> animationElts = animationsElt->find_children("animation");
	int animationCount = static_cast<int>(animationElts.size());

	std::map<std::string,Animation_Ptr> animations;
	for(int i=0; i<animationCount; ++i)
	{
		const XMLElement_CPtr& animationElt = animationElts[i];

		// Load in the tracks for the bones.
		XMLElement_CPtr tracksElt = animationElt->find_unique_child("tracks");
		std::vector<XMLElement_CPtr> trackElts = tracksElt->find_children("track");
		int trackCount = static_cast<int>(trackElts.size());
		int keyframeCount = 0;

		typedef std::vector<Matrix44_Ptr> Track;
		std::map<std::string,Track> tracks;

		for(int j=0; j<trackCount; ++j)
		{
			const XMLElement_CPtr& trackElt = trackElts[j];

			std::string bone = trackElt->attribute("bone");

			// Read in the bone keyframes for this particular bone.
			XMLElement_CPtr keyframesElt = trackElt->find_unique_child("keyframes");
			std::vector<XMLElement_CPtr> keyframeElts = keyframesElt->find_children("keyframe");
			keyframeCount = static_cast<int>(keyframeElts.size());

			Track track(keyframeCount);
			for(int k=0; k<keyframeCount; ++k)
			{
				const XMLElement_CPtr& keyframeElt = keyframeElts[k];

				XMLElement_CPtr translateElt = keyframeElt->find_unique_child("translate");
				Vector3d translation = extract_vector3d(translateElt);

				XMLElement_CPtr rotateElt = keyframeElt->find_unique_child("rotate");
				double rotateAngle = lexical_cast<double,std::string>(rotateElt->attribute("angle"));
				XMLElement_CPtr axisElt = rotateElt->find_unique_child("axis");
				Vector3d rotateAxis = extract_vector3d(axisElt);

				// TODO: Make use of scale here as well if necessary.

				track[k] = Matrix44::from_axis_angle_translation(rotateAxis, rotateAngle, translation);
			}
			tracks.insert(std::make_pair(bone,track));
		}

		// Check that each track has the same number of bone keyframes.
		for(std::map<std::string,Track>::const_iterator kt=tracks.begin(), kend=tracks.end(); kt!=kend; ++kt)
		{
			const Track& track = kt->second;
			if(track.size() != keyframeCount) throw Exception("Bad track length");
		}

		// Use the tracks to create the *model* keyframes (note: these are distinct from the bone keyframes!).
		std::vector<Keyframe_Ptr> keyframes(keyframeCount);
		for(int j=0; j<keyframeCount; ++j)
		{
			std::vector<Matrix44_CPtr> boneMatrices(boneCount);
			for(int k=0; k<boneCount; ++k)
			{
				Bone_CPtr bone = boneConfiguration->bones(k);
				std::map<std::string,Track>::const_iterator kt = tracks.find(bone->name());
				if(kt != tracks.end())
				{
					// If there's an animation track for this bone, use the track matrix.
					const Track& track = kt->second;
					boneMatrices[k] = track[j];
				}
				else
				{
					// Otherwise, use the identity matrix.
					boneMatrices[k] = Matrix44::identity();
				}
			}

			keyframes[j].reset(new Keyframe(boneMatrices));
		}

		std::string name = animationElt->attribute("name");
		double length = lexical_cast<double,std::string>(animationElt->attribute("length"));
		Animation_Ptr animation(new Animation(length, keyframes));
		animations.insert(std::make_pair(name, animation));
	}

	return Skeleton_Ptr(new Skeleton(boneConfiguration, animations));
}
catch(bad_lexical_cast&)
{
	throw Exception("An element attribute was not of the correct type");
}

}
