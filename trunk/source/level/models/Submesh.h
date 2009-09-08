/***
 * hesperus: Submesh.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SUBMESH
#define H_HESP_SUBMESH

#include <vector>

#include <source/materials/Material.h>
#include <source/math/vectors/TexCoords.h>
#include <source/ogl/WrappedGL.h>
#include <source/textures/Texture.h>
#include "ModelVertex.h"
#include "Skeleton.h"

namespace hesp {

class Submesh
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<unsigned int> m_vertIndices;
	std::vector<ModelVertex> m_vertices;

	Material_Ptr m_material;
	std::vector<GLdouble> m_texCoordArray;	// the tex coord array is empty if we're using a basic material

	// Note: The vertex array is created as necessary using the skin() method.
	std::vector<GLdouble> m_vertArray;

	//#################### CONSTRUCTORS ####################
public:
	Submesh(const std::vector<unsigned int>& vertIndices, const std::vector<ModelVertex>& vertices, const Material_Ptr& material, const std::vector<TexCoords>& texCoords);

	//#################### PUBLIC METHODS ####################
public:
	void render() const;
	void skin(const Skeleton_CPtr& skeleton);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Submesh> Submesh_Ptr;
typedef shared_ptr<const Submesh> Submesh_CPtr;

}

#endif
