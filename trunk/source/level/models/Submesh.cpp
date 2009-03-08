/***
 * hesperus: Submesh.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Submesh.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Submesh::Submesh(const Material& material, const std::vector<int>& vertIndices, const std::vector<ModelVertex>& vertices)
:	m_material(material), m_vertIndices(vertIndices), m_vertices(vertices),
	m_vertexArray(vertices.size() * 3), m_normalArray(vertices.size() * 3)
{}

//#################### PUBLIC METHODS ####################
void Submesh::render() const
{
	// NYI
	throw 23;
}

void Submesh::skin(const Skeleton_Ptr& skeleton)
{
	int vertCount = static_cast<int>(m_vertices.size());
	for(int i=0; i<vertCount; ++i)
	{
		const Vector3d& v = m_vertices[i].position();
		const std::vector<BoneWeight>& boneWeights = m_vertices[i].bone_weights();

		// TODO
	}

	// NYI
	throw 23;
}

}
