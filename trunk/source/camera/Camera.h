/***
 * hesperus: Camera.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CAMERA
#define H_HESP_CAMERA

#include <source/math/vectors/Vector3.h>

namespace hesp {

/**
This class represents a camera for 3D views. Cameras are defined with
a position and three mutually-orthogonal axes, namely n (points in the
direction faced by the camera), u (points to the left of the camera)
and v (points to the top of the camera).
*/
class Camera
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_position, m_nVector, m_uVector, m_vVector;

	//#################### CONSTRUCTORS ####################
public:
	Camera(const Vector3d& position, const Vector3d& look, const Vector3d& up);

	//#################### PUBLIC METHODS ####################
public:
	void draw_axes() const;
	void move_n(double delta);
	void move_u(double delta);
	void move_v(double delta);
	const Vector3d& n() const;
	const Vector3d& position() const;
	void rotate(const Vector3d& axis, double angle);
	void set_position(const Vector3d& position);
	void set_view() const;
	const Vector3d& u() const;
	const Vector3d& v() const;
};

}

#endif
