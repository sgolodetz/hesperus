/***
 * hesperus: Contact.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CONTACT
#define H_HESP_CONTACT

#include <boost/optional.hpp>

#include <source/math/vectors/Vector3.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class PhysicsObject;

class Contact
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_normal;							// the normal at the contact point
	PhysicsObject& m_objectA;					// the first object involved in the collision
	boost::optional<PhysicsObject&> m_objectB;	// the second object involved in the collision (if any)
	Vector3d m_relativePointA;					// a relative point on object A representing the contact
	Vector3d m_relativePointB;					// a relative point on object B representing the contact
												// (if B is the world, then this is an absolute point)
	double m_time;								// a number in [0,1] indicating when in the frame the contact occurred

	//#################### CONSTRUCTORS ####################
public:
	Contact(const Vector3d& relativePointA, const Vector3d& relativePointB, const Vector3d& normal, double time, PhysicsObject& objectA, const boost::optional<PhysicsObject&>& objectB = boost::none);

	//#################### PUBLIC METHODS ####################
public:
	const Vector3d& normal() const;
	PhysicsObject& objectA() const;
	const boost::optional<PhysicsObject&>& objectB() const;
	double penetration_depth() const;
	Vector3d pointA() const;
	Vector3d pointB() const;
	const Vector3d& relative_pointA() const;
	const Vector3d& relative_pointB() const;
	double time() const;
};

}

#endif
