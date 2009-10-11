/***
 * hesperus: Contact.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Contact.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Contact::Contact(const Vector3d& point, const Vector3d& normal, double penetrationDepth, double time,
				 PhysicsObject& object1, const boost::optional<PhysicsObject&>& object2)
:	m_point(point), m_normal(normal), m_penetrationDepth(penetrationDepth), m_time(time),
	m_object1(object1), m_object2(object2)
{}

//#################### PUBLIC METHODS ####################
const Vector3d& Contact::normal() const							{ return m_normal; }
PhysicsObject& Contact::object1() const							{ return m_object1; }
const boost::optional<PhysicsObject&>& Contact::object2() const	{ return m_object2; }
double Contact::penetration_depth() const						{ return m_penetrationDepth; }
const Vector3d& Contact::point() const							{ return m_point; }
double Contact::time() const									{ return m_time; }

}
