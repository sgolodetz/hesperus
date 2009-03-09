/***
 * hesperus: Submesh.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SUBMESH
#define H_HESP_SUBMESH

#include <vector>

#include <source/ogl/WrappedGL.h>

#include "Material.h"
#include "ModelVertex.h"
#include "Skeleton.h"

namespace hesp {

class Submesh
{
	//#################### PRIVATE VARIABLES ####################
private:
	Material m_material;
	std::vector<unsigned int> m_vertIndices;
	std::vector<ModelVertex> m_vertices;

	// Note: This is created as necessary using the skin() method.
	std::vector<GLdouble> m_vertArray;

	//#################### CONSTRUCTORS ####################
public:
	Submesh(const Material& material, const std::vector<unsigned int>& vertIndices, const std::vector<ModelVertex>& vertices);

	//#################### PUBLIC METHODS ####################
public:
	void render() const;
	void skin(const Skeleton_Ptr& skeleton);

	//#################### PRIVATE METHODS ####################
private:
	static Vector3d apply_rbt(const Matrix44_Ptr& rbt, const Vector3d& p);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Submesh> Submesh_Ptr;
typedef shared_ptr<const Submesh> Submesh_CPtr;

}

#endif
