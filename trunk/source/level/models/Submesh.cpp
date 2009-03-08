/***
 * hesperus: Submesh.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Submesh.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Submesh::Submesh(const Material& material, const std::vector<int>& vertIndices, const std::vector<ModelVertex>& vertices)
:	m_material(material), m_vertIndices(vertIndices), m_vertices(vertices),
	m_vertexArray(vertices.size() * 3)
{}

//#################### PUBLIC METHODS ####################
void Submesh::render() const
{
	// NYI
	throw 23;
}

void Submesh::skin(const Skeleton_Ptr& skeleton)
{
	/*
	Linear Blend Skinning Algorithm:

	P = sum_i w_i * M_i * M_{0,i}^{-1} P_0

	Each M_{0,i}^{-1} matrix gets P_0 (the rest vertex) into its corresponding bone's coordinate frame.
	We construct matrices M_n * M_{0,n}^-1 for each n in advance to avoid repeating calculations.
	I refer to these in the code as the 'skinning matrices'.
	*/

	BoneConfiguration_Ptr boneConfiguration = skeleton->bone_configuration();
	int boneCount = boneConfiguration->bone_count();

	// Construct the skinning matrices.
	std::vector<Matrix44_Ptr> skinningMatrices(boneCount);
	for(int i=0; i<boneCount; ++i)
	{
		skinningMatrices[i] = boneConfiguration->bones(i)->absolute_matrix() * skeleton->to_bone_matrix(i);
	}

	// Build the vertex array.
	int vertCount = static_cast<int>(m_vertices.size());
	for(int i=0; i<vertCount; ++i)
	{
		const Vector3d& v = m_vertices[i].position();
		const std::vector<BoneWeight>& boneWeights = m_vertices[i].bone_weights();

		int boneWeightCount = static_cast<int>(boneWeights.size());
		for(int j=0; j<boneWeightCount; ++j)
		{
			int boneIndex = boneWeights[j].bone_index();
			double weight = boneWeights[j].weight();

			// TODO
		}
	}

	// NYI
	throw 23;
}

}
