/***
 * hesperus: AbsorbProjectileContactResolver.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AbsorbProjectileContactResolver.h"

#include <source/level/objects/base/ObjectManager.h>
#include <source/level/physics/Contact.h>
#include <source/level/physics/PhysicsObject.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
AbsorbProjectileContactResolver::AbsorbProjectileContactResolver(ObjectManager *objectManager, PhysicsMaterial projectileMaterial)
:	m_objectManager(objectManager), m_projectileMaterial(projectileMaterial)
{}

//#################### PRIVATE METHODS ####################
void AbsorbProjectileContactResolver::resolve_object_object(const Contact& contact, const OnionTree_CPtr& tree) const
{
	// FIXME: Need to check that the object other than the projectile is not the one that fired the projectile!
	ObjectID projectile;
	if(contact.objectA().material() == m_projectileMaterial) projectile = contact.objectA().owner();
	else if(contact.objectB()->material() == m_projectileMaterial) projectile = contact.objectB()->owner();
	else throw Exception("Inappropriate contact resolver: Neither of the objects involved in the contact had the specified projectile type");

	m_objectManager->queue_for_destruction(projectile);
}

void AbsorbProjectileContactResolver::resolve_object_world(const Contact& contact) const
{
	m_objectManager->queue_for_destruction(contact.objectA().owner());
}

}
