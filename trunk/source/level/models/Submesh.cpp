/***
 * hesperus: Submesh.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Submesh.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Submesh::Submesh(const Material& material, const std::vector<int>& vertIndices, const std::vector<ModelVertex>& vertices)
:	m_material(material), m_vertIndices(vertIndices), m_vertices(vertices),
	m_vertArray(vertices.size() * 3)
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

	P = (\sum_i w_i * M_i * M_{0,i}^{-1}) * P_0 / (sum i w_i)

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
	for(int i=0, offset=0; i<vertCount; ++i, offset+=3)
	{
		const Vector3d& p0 = m_vertices[i].position();
		const std::vector<BoneWeight>& boneWeights = m_vertices[i].bone_weights();
		int boneWeightCount = static_cast<int>(boneWeights.size());

		Vector3d p;
		if(boneWeightCount != 0)
		{
			double boneWeightSum = 0;

			Matrix44_Ptr m = Matrix44::zeros();		// used as an accumulator for \sum_i w_i * M_i * M_{0,i}^{-1}

			for(int j=0; j<boneWeightCount; ++j)
			{
				int boneIndex = boneWeights[j].bone_index();
				double boneWeight = boneWeights[j].weight();
				boneWeightSum += boneWeight;
				m += boneWeight * skinningMatrices[boneIndex];
			}

			// Note: This is effectively p = m*p0 (if we think of p0 as (p0.x, p0.y, p0.z, 1)).
			p = apply_rbt(m, p0);
			p /= boneWeightSum;
		}
		else
		{
			// If this vertex is unaffected by the armature (i.e. no bone weights have been assigned to it),
			// use its rest position as its real position (it's the best we can do).
			p = p0;
		}

		m_vertArray[offset] = p.x;
		m_vertArray[offset+1] = p.y;
		m_vertArray[offset+2] = p.z;
	}
}

//#################### PRIVATE METHODS ####################
/**
Applies a 4x4 rigid-body transform matrix to a 3D *point* (note: w = 1 for a point,
but 0 for a vector - this function is only suitable for the former).

@param rbt	The rigid-body transform matrix
@param p	The point to which to apply it
@return		The transformed point
*/
Vector3d Submesh::apply_rbt(const Matrix44_Ptr& rbt, const Vector3d& p)
{
	return Vector3d(
		(*rbt)(0,0) * p.x + (*rbt)(0,1) * p.y + (*rbt)(0,2) * p.z + (*rbt)(0,3),
		(*rbt)(1,0) * p.x + (*rbt)(1,1) * p.y + (*rbt)(1,2) * p.z + (*rbt)(1,3),
		(*rbt)(2,0) * p.x + (*rbt)(2,1) * p.y + (*rbt)(2,2) * p.z + (*rbt)(2,3)
	);
}

}
