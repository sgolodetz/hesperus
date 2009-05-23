/***
 * hesperus: Camera.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CAMERA
#define H_HESP_CAMERA

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/vectors/Vector3.h>

namespace hesp {

class Camera
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~Camera() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual bool is_inside_viewer() const = 0;

	/**
	@return	The calculated "eye" position of the camera
	*/
	virtual Vector3d set_view() const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Camera> Camera_Ptr;
typedef shared_ptr<const Camera> Camera_CPtr;

}

#endif
