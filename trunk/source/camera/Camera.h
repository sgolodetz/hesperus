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
	virtual void move_n(double delta) = 0;
	virtual void move_u(double delta) = 0;
	virtual void move_v(double delta) = 0;
	virtual const Vector3d& n() const = 0;
	virtual const Vector3d& position() const = 0;
	virtual void rotate(const Vector3d& axis, double angle) = 0;
	virtual void set_position(const Vector3d& position) = 0;
	virtual const Vector3d& u() const = 0;
	virtual const Vector3d& v() const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Camera> Camera_Ptr;

}

#endif
