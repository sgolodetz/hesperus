/***
 * hesperus: VariableCameraComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_VARIABLECAMERACOMPONENT
#define H_HESP_VARIABLECAMERACOMPONENT

#include "ICameraComponent.h"

namespace hesp {

class VariableCameraComponent : public ICameraComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	Camera_Ptr m_camera;

	//#################### CONSTRUCTORS ####################
public:
	VariableCameraComponent(std::istream& is);

	//#################### PUBLIC METHODS ####################
public:
	Camera& camera();
	const Camera& camera() const;
	void save(std::ostream& os) const;
};

}

#endif
