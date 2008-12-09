/***
 * hesperus: Texture.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_TEXTURES_TEXTURE
#define H_HESP_TEXTURES_TEXTURE

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/ogl/WrappedGL.h>

namespace hesp {

/**
This class represents OpenGL textures. Essentially it's just a simple wrapper for an OpenGL texture ID.
*/
class Texture
{
	//#################### FRIENDS ####################
	friend class TextureFactory;

	//#################### PRIVATE VARIABLES ####################
private:
	shared_ptr<GLuint> m_id;

	//#################### CONSTRUCTORS ####################
private:
	Texture(GLuint id);

	//#################### PUBLIC METHODS ####################
public:
	void bind() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Texture> Texture_Ptr;
typedef shared_ptr<const Texture> Texture_CPtr;

}

#endif
