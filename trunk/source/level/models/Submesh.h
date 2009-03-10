/***
 * hesperus: Submesh.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SUBMESH
#define H_HESP_SUBMESH

#include <vector>

#include <source/math/vectors/TexCoords.h>
#include <source/ogl/WrappedGL.h>
#include <source/textures/Texture.h>

#include "Material.h"
#include "ModelVertex.h"
#include "Skeleton.h"

namespace hesp {

class Submesh
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<unsigned int> m_vertIndices;
	std::vector<ModelVertex> m_vertices;

	// Note: We're either using a material, or a texture, but never both at once.
	Material_Ptr m_material;
	Texture_Ptr m_texture;
	std::vector<GLdouble> m_texCoordArray;	// the tex coord array is empty if we're using a material

	// Note: The vertex array is created as necessary using the skin() method.
	std::vector<GLdouble> m_vertArray;

	//#################### CONSTRUCTORS ####################
public:
	Submesh(const std::vector<unsigned int>& vertIndices, const std::vector<ModelVertex>& vertices, const Material_Ptr& material);
	Submesh(const std::vector<unsigned int>& vertIndices, const std::vector<ModelVertex>& vertices, const Texture_Ptr& texture, const std::vector<TexCoords>& texCoords);

	//#################### PUBLIC METHODS ####################
public:
	void render() const;
	void skin(const Skeleton_Ptr& skeleton);

	//#################### PRIVATE METHODS ####################
private:
	static Vector3d apply_rbt(const Matrix44_Ptr& rbt, const Vector3d& p);
	void render_using_material() const;
	void render_using_texture() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Submesh> Submesh_Ptr;
typedef shared_ptr<const Submesh> Submesh_CPtr;

}

#endif
