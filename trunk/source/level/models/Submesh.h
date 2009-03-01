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
	// TODO
	std::vector<ModelVertex> m_vertices;

	// TODO
};

}

#endif
