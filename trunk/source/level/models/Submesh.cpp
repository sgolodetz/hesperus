/***
 * hesperus: Submesh.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Submesh.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Submesh::Submesh(const Material& material, const std::vector<int>& vertIndices, const std::vector<ModelVertex>& vertices)
:	m_material(material), m_vertIndices(vertIndices), m_vertices(vertices)
{}

}
