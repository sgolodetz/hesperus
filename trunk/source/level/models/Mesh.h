/***
 * hesperus: Mesh.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MESH
#define H_HESP_MESH

#include "Submesh.h"

namespace hesp {

class Mesh
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<Submesh_Ptr> m_submeshes;

	//#################### CONSTRUCTORS ####################
public:
	Mesh(const std::vector<Submesh_Ptr>& submeshes);

	//#################### PUBLIC METHODS ####################
public:
	void render() const;
	void skin(const BoneConfiguration_Ptr& boneConfiguration);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Mesh> Mesh_Ptr;
typedef shared_ptr<const Mesh> Mesh_CPtr;

}

#endif
