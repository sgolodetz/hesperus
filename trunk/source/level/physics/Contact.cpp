/***
 * hesperus: Contact.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Contact.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Contact::Contact(const Vector3d& point, const Vector3d& normal, double penetrationDepth, double time,
				 PhysicsObject& objectA, const boost::optional<PhysicsObject&>& objectB)
:	m_point(point), m_normal(normal), m_penetrationDepth(penetrationDepth), m_time(time),
	m_objectA(objectA), m_objectB(objectB)
{}

//#################### PUBLIC METHODS ####################
const Vector3d& Contact::normal() const							{ return m_normal; }
PhysicsObject& Contact::objectA() const							{ return m_objectA; }
const boost::optional<PhysicsObject&>& Contact::objectB() const	{ return m_objectB; }
double Contact::penetration_depth() const						{ return m_penetrationDepth; }
const Vector3d& Contact::point() const							{ return m_point; }
double Contact::time() const									{ return m_time; }

}
