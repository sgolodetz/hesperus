/***
 * hesperus: BasicContactResolver.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BasicContactResolver.h"

#include <source/math/Constants.h>
#include "Contact.h"
#include "PhysicsObject.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BasicContactResolver::BasicContactResolver(double restitutionCoefficient)
:	m_restitutionCoefficient(restitutionCoefficient)
{}

//#################### PUBLIC METHODS ####################
void BasicContactResolver::resolve_contact(const Contact& contact) const
{
	if(contact.objectB())	resolve_object_object(contact);
	else					resolve_object_world(contact);
}

//#################### PRIVATE METHODS ####################
void BasicContactResolver::resolve_object_object(const Contact& contact) const
{
	// Step 1:	Calculate what fractions of the resolution will apply to each of the two objects involved.
	PhysicsObject& objectA = contact.objectA();
	PhysicsObject& objectB = *contact.objectB();
	double invMassA = objectA.inverse_mass();
	double invMassB = objectB.inverse_mass();
	double totalInvMass = invMassA + invMassB;

	// We can't resolve a collision between two objects with infinite mass, since we can't move either of them.
	if(totalInvMass < EPSILON) return;

	double fractionA = invMassA / totalInvMass;
	double fractionB = invMassB / totalInvMass;

	// Step 2:	Resolve interpenetration. For object-object contacts, this involves moving the
	//			objects apart along the contact normal in proportion to their inverse masses.
	double penetrationDepth = contact.penetration_depth();
	double depthA = penetrationDepth * fractionA;
	double depthB = penetrationDepth * fractionB;

	// Update the positions of the objects to resolve the interpenetration.
	objectA.set_position(objectA.position() + contact.normal() * depthA);
	objectB.set_position(objectB.position() - contact.normal() * depthB);

	// Step 3:	Determine the new velocities of the objects.

	// Calculate the old separating velocity (in the normal direction).
	double oldSepVelocity = (objectA.velocity() - objectB.velocity()).dot(contact.normal());

	// If the contact is either separating or stationary, there's nothing to do.
	if(oldSepVelocity >= 0) return;

	// Calculate the new separating velocity (in the normal direction).
	double newSepVelocity = -m_restitutionCoefficient * oldSepVelocity;

	// Update the velocities of the objects.
	double deltaVel = newSepVelocity - oldSepVelocity;
	Vector3d deltaVelA = deltaVel * fractionA * contact.normal();
	Vector3d deltaVelB = deltaVel * fractionB * -contact.normal();
	objectA.set_velocity(objectA.velocity() + deltaVelA);
	objectB.set_velocity(objectB.velocity() + deltaVelB);
}

void BasicContactResolver::resolve_object_world(const Contact& contact) const
{
	// Step 1:	Resolve interpenetration. For object-world contacts, this involves moving the object
	//			back to the transition location, which is stored as point B in the contact structure.
	PhysicsObject& objectA = contact.objectA();
	objectA.set_position(contact.pointB());

	// Step 2:	Determine the new velocity of the object.

	// Calculate the old separating velocity (in the normal direction).
	double oldSepVelocity = objectA.velocity().dot(contact.normal());

	// If the contact is either separating or stationary, there's nothing to do.
	if(oldSepVelocity >= 0) return;

	// Calculate the new separating velocity (in the normal direction).
	double newSepVelocity = -m_restitutionCoefficient * oldSepVelocity;

	// Update the velocity of the object.
	Vector3d deltaVelA = (newSepVelocity - oldSepVelocity) * contact.normal();
	objectA.set_velocity(objectA.velocity() + deltaVelA);
}

}
