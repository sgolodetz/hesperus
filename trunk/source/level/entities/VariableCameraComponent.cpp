/***
 * hesperus: VariableCameraComponent.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "VariableCameraComponent.h"

#include <source/camera/VariableCamera.h>
#include <source/io/FieldIO.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
VariableCameraComponent::VariableCameraComponent(std::istream& is)
{
	Vector3d position = FieldIO::read_typed_field<Vector3d>(is, "Position");
	Vector3d look = FieldIO::read_typed_field<Vector3d>(is, "Look");
	m_camera.reset(new VariableCamera(position, look));
}

//#################### PUBLIC METHODS ####################
Camera& VariableCameraComponent::camera()
{
	return *m_camera;
}

const Camera& VariableCameraComponent::camera() const
{
	return *m_camera;
}

void VariableCameraComponent::save(std::ostream& os) const
{
	FieldIO::write_typed_field(os, "Position", m_camera->position());
	FieldIO::write_typed_field(os, "Look", m_camera->n());
}

}
