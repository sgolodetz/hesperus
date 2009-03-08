/***
 * hesperus: Mesh.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Mesh.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Mesh::Mesh(const std::vector<Submesh_Ptr>& submeshes)
:	m_submeshes(submeshes)
{}

}
