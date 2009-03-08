/***
 * hesperus: Submesh.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SUBMESH
#define H_HESP_SUBMESH

#include <vector>

#include "Material.h"
#include "ModelVertex.h"

namespace hesp {

class Submesh
{
	//#################### PRIVATE VARIABLES ####################
private:
	Material m_material;
	std::vector<int> m_vertIndices;
	std::vector<ModelVertex> m_vertices;

	//#################### CONSTRUCTORS ####################
public:
	Submesh(const Material& material, const std::vector<int>& vertIndices, const std::vector<ModelVertex>& vertices);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Submesh> Submesh_Ptr;
typedef shared_ptr<const Submesh> Submesh_CPtr;

}

#endif
