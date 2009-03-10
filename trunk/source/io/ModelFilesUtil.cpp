/***
 * hesperus: ModelFilesUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ModelFilesUtil.h"

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>
#include <source/images/BitmapLoader.h>
#include <source/textures/TextureFactory.h>

namespace {

//#################### CONSTANTS ####################
const double SCALE = 1.0/10;	// the models are built such that 10 units in Blender corresponds to 1 unit in the game

}

namespace hesp {

//#################### LOADING SUPPORT METHODS ####################
TexCoords ModelFilesUtil::extract_texcoords(const XMLElement_CPtr& elt)
{
	double u = lexical_cast<double,std::string>(elt->attribute("u"));
	double v = lexical_cast<double,std::string>(elt->attribute("v"));
	return TexCoords(u,v);
}

Vector3d ModelFilesUtil::extract_vector3d(const XMLElement_CPtr& elt)
{
	double x = lexical_cast<double,std::string>(elt->attribute("x"));
	double y = lexical_cast<double,std::string>(elt->attribute("y"));
	double z = lexical_cast<double,std::string>(elt->attribute("z"));
	return Vector3d(x,y,z);
}

Mesh_Ptr ModelFilesUtil::load_mesh(const std::string& filename)
try
{
	XMLLexer_Ptr lexer(new XMLLexer(filename));
	XMLParser parser(lexer);
	XMLElement_CPtr root = parser.parse();
	XMLElement_CPtr meshElt = root->find_unique_child("mesh");

	XMLElement_CPtr submeshesElt = meshElt->find_unique_child("submeshes");

	std::vector<XMLElement_CPtr> submeshElts = submeshesElt->find_children("submesh");
	int submeshCount = static_cast<int>(submeshElts.size());

	std::vector<Submesh_Ptr> submeshes;
	for(int i=0; i<submeshCount; ++i)
	{
		const XMLElement_CPtr& submeshElt = submeshElts[i];

		std::string material = submeshElt->attribute("material");

		// Read in the vertex indices for the triangles in the mesh.
		XMLElement_CPtr facesElt = submeshElt->find_unique_child("faces");
		std::vector<XMLElement_CPtr> faceElts = facesElt->find_children("face");
		int faceCount = static_cast<int>(faceElts.size());

		std::vector<unsigned int> vertIndices;
		vertIndices.reserve(faceCount * 3);

		for(int j=0; j<faceCount; ++j)
		{
			const XMLElement_CPtr& faceElt = faceElts[j];
			unsigned int v1 = lexical_cast<unsigned int,std::string>(faceElt->attribute("v1"));
			unsigned int v2 = lexical_cast<unsigned int,std::string>(faceElt->attribute("v2"));
			unsigned int v3 = lexical_cast<unsigned int,std::string>(faceElt->attribute("v3"));
			vertIndices.push_back(v1);
			vertIndices.push_back(v2);
			vertIndices.push_back(v3);
		}

		// Read in the vertex positions and normals.
		XMLElement_CPtr geometryElt = submeshElt->find_unique_child("geometry");
		XMLElement_CPtr vertexbufferElt = geometryElt->find_unique_child("vertexbuffer");
		if(!vertexbufferElt->has_attribute("positions") || vertexbufferElt->attribute("positions") != "true" ||
		   !vertexbufferElt->has_attribute("normals") || vertexbufferElt->attribute("normals") != "true")
		{
			throw Exception("Both vertex positions and normals are required to be present - did you make sure to export them?");
		}

		bool useTexture = vertexbufferElt->has_attribute("texture_coords") && vertexbufferElt->attribute("texture_coords") == "1";

		std::vector<XMLElement_CPtr> vertexElts = vertexbufferElt->find_children("vertex");
		int vertCount = static_cast<int>(vertexElts.size());

		std::vector<ModelVertex> vertices;
		vertices.reserve(vertCount);

		for(int j=0; j<vertCount; ++j)
		{
			const XMLElement_CPtr& vertexElt = vertexElts[j];
			XMLElement_CPtr positionElt = vertexElt->find_unique_child("position");
			XMLElement_CPtr normalElt = vertexElt->find_unique_child("normal");
			Vector3d position = extract_vector3d(positionElt) * SCALE;
			Vector3d normal = extract_vector3d(normalElt);

			vertices.push_back(ModelVertex(position, normal));
		}

		std::vector<TexCoords> texCoords;
		if(useTexture)
		{
			texCoords.reserve(vertCount);
			for(int j=0; j<vertCount; ++j)
			{
				const XMLElement_CPtr& vertexElt = vertexElts[j];
				XMLElement_CPtr texcoordElt = vertexElt->find_unique_child("texcoord");
				texCoords.push_back(extract_texcoords(texcoordElt));
			}
		}

		// Read in the vertex bone assignments.
		XMLElement_CPtr boneassignmentsElt = submeshElt->find_unique_child("boneassignments");
		std::vector<XMLElement_CPtr> vertexboneassignmentElts = boneassignmentsElt->find_children("vertexboneassignment");
		int boneassignmentCount = static_cast<int>(vertexboneassignmentElts.size());

		for(int j=0; j<boneassignmentCount; ++j)
		{
			const XMLElement_CPtr& vbaElt = vertexboneassignmentElts[j];

			int vertIndex = lexical_cast<int,std::string>(vbaElt->attribute("vertexindex"));
			if(vertIndex < 0 || vertIndex >= vertCount)
				throw Exception("Invalid vertex index in bone assignment " + lexical_cast<std::string,int>(j));

			int boneIndex = lexical_cast<int,std::string>(vbaElt->attribute("boneindex"));
			double weight = lexical_cast<double,std::string>(vbaElt->attribute("weight"));

			vertices[vertIndex].add_bone_weight(BoneWeight(boneIndex, weight));
		}

		if(useTexture)
		{
			// FIXME: Work out which texture to load by reading the .material file.
			Texture_Ptr tempTexture = TextureFactory::create_texture24(BitmapLoader::load_image24("resources/models/UV.bmp"));

			submeshes.push_back(Submesh_Ptr(new Submesh(vertIndices, vertices, tempTexture, texCoords)));
		}
		else
		{
			// FIXME: Obtain the material from the .material file.
			Material_Ptr tempMaterial(new Material(Colour3d(0.5,0.5,0.5), Colour3d(0.8,0.8,0.64), Colour3d(0.5,0.5,0.5), 12.5, Colour3d(0,0,0)));

			submeshes.push_back(Submesh_Ptr(new Submesh(vertIndices, vertices, tempMaterial)));
		}
	}

	return Mesh_Ptr(new Mesh(submeshes));
}
catch(bad_lexical_cast&)
{
	throw Exception("An element attribute was not of the correct type");
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
		Vector3d position = extract_vector3d(positionElt) * SCALE;

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
				Vector3d translation = extract_vector3d(translateElt) * SCALE;

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
			std::vector<Matrix44_Ptr> boneMatrices(boneCount);
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
