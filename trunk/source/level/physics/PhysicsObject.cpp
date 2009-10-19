/***
 * hesperus: PhysicsObject.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PhysicsObject.h"

#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
PhysicsObject::PhysicsObject(double inverseMass, PhysicsMaterial material, const Vector3d& position,
							 const Vector3d& velocity)
:	m_id(-1), m_inverseMass(inverseMass), m_material(material), m_position(position), m_velocity(velocity), m_sleeping(false)
{}

//#################### DESTRUCTOR ####################
PhysicsObject::~PhysicsObject() {}

//#################### PUBLIC METHODS ####################
double PhysicsObject::inverse_mass() const									{ return m_inverseMass; }
PhysicsMaterial PhysicsObject::material() const								{ return m_material; }
const Vector3d& PhysicsObject::position() const								{ return m_position; }
const std::list<Plane>& PhysicsObject::recent_planes() const				{ return m_recentPlanes; }

void PhysicsObject::set_position(Vector3d position)
{
	m_previousPosition = m_position;
	m_position = position;
}

void PhysicsObject::set_velocity(const Vector3d& velocity)					{ m_velocity = velocity; }

void PhysicsObject::update_recent_planes(const Plane& plane)
{
	// Remove any recent planes which the object's no longer on.
	for(std::list<Plane>::iterator it=m_recentPlanes.begin(), iend=m_recentPlanes.end(); it!=iend;)
	{
		if(classify_point_against_plane(m_position, *it) == CP_COPLANAR) ++it;
		else it = m_recentPlanes.erase(it);
	}

	// Add the latest plane.
	m_recentPlanes.push_front(plane);
}

const Vector3d& PhysicsObject::velocity() const								{ return m_velocity; }

//#################### PROTECTED METHODS ####################
const Vector3d& PhysicsObject::accumulated_force() const					{ return m_accumulatedForce; }

//#################### PRIVATE METHODS ####################
void PhysicsObject::apply_force(const Vector3d& force)						{ m_accumulatedForce += force; }
void PhysicsObject::clear_accumulated_force()								{ m_accumulatedForce = Vector3d(0,0,0); }
int PhysicsObject::id() const												{ return m_id; }
bool PhysicsObject::is_sleeping() const										{ return m_sleeping; }
const boost::optional<Vector3d>& PhysicsObject::previous_position() const	{ return m_previousPosition; }
void PhysicsObject::set_id(int id)											{ m_id = id; }
void PhysicsObject::set_sleeping(bool sleeping)								{ m_sleeping = sleeping; }

}
