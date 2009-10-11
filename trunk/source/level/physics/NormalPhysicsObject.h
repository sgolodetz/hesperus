/***
 * hesperus: NormalPhysicsObject.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NORMALPHYSICSOBJECT
#define H_HESP_NORMALPHYSICSOBJECT

#include "PhysicsObject.h"

namespace hesp {

class NormalPhysicsObject : public PhysicsObject
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_boundsGroup;
	std::string m_posture;

	//#################### CONSTRUCTORS ####################
public:
	NormalPhysicsObject(const std::string& boundsGroup, const std::string& posture, double inverseMass, PhysicsMaterial material, const Vector3d& position, const Vector3d& velocity = Vector3d(0,0,0));

	//#################### PUBLIC METHODS ####################
public:
	Bounds_CPtr bounds(const BoundsManager_CPtr& boundsManager) const;
	const std::string& bounds_group() const;
	const std::string& posture() const;
	void set_posture(const std::string& posture);
	void update(int milliseconds);
};

}

#endif
