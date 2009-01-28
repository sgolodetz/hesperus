/***
 * hesperus: VariableCameraComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_VARIABLECAMERACOMPONENT
#define H_HESP_VARIABLECAMERACOMPONENT

#include <source/camera/VariableCamera.h>
#include <source/io/FieldIO.h>
#include "ICameraComponent.h"

namespace hesp {

class VariableCameraComponent : public ICameraComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	Camera_Ptr m_camera;

	//#################### CONSTRUCTORS ####################
public:
	VariableCameraComponent(std::istream& is)
	{
		Vector3d position = FieldIO::read_typed_field<Vector3d>(is, "Position");
		Vector3d look = FieldIO::read_typed_field<Vector3d>(is, "Look");
		m_camera.reset(new VariableCamera(position, look));
	}

	//#################### PUBLIC METHODS ####################
public:
	Camera& camera()
	{
		return *m_camera;
	}

	const Camera& camera() const
	{
		return *m_camera;
	}

	void save(std::ostream& os) const
	{
		FieldIO::write_typed_field(os, "Position", m_camera->position());
		FieldIO::write_typed_field(os, "Look", m_camera->n());
	}
};

}

#endif
