/***
 * hesperus: Texture.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_TEXTURE
#define H_HESP_TEXTURE

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/ogl/WrappedGL.h>

namespace hesp {

/**
This class represents OpenGL textures. Essentially it's just a simple wrapper for an OpenGL texture ID,
but with reloading capabilities (i.e. the texture will reload itself if the screen resolution is changed).
*/
class Texture
{
	//#################### PROTECTED VARIABLES ####################
protected:
	mutable shared_ptr<GLuint> m_id;

	//#################### CONSTRUCTORS ####################
protected:
	Texture();

	//#################### DESTRUCTOR ####################
public:
	virtual ~Texture();

	//#################### PROTECTED ABSTRACT METHODS ####################
protected:
	virtual void reload() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	void bind() const;

	//#################### PROTECTED METHODS ####################
protected:
	void set_id(GLuint id) const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Texture> Texture_Ptr;
typedef shared_ptr<const Texture> Texture_CPtr;

}

#endif
