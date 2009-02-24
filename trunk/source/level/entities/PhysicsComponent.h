/***
 * hesperus: PhysicsComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PHYSICSCOMPONENT
#define H_HESP_PHYSICSCOMPONENT

#include "IPhysicsComponent.h"

namespace hesp {

class PhysicsComponent : public IPhysicsComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	double m_mass;			// the mass of the entity in kg
	Vector3d m_velocity;	// the velocity of the entity in m/s

	//#################### CONSTRUCTORS ####################
public:
	PhysicsComponent(std::istream& is);

	//#################### PUBLIC METHODS ####################
public:
	double mass() const;
	double run_speed() const;
	void save(std::ostream& os) const;
	void set_velocity(const Vector3d& velocity);
	const Vector3d& velocity() const;
	double walk_speed() const;
};

}

#endif
