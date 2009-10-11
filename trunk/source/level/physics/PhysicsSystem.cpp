/***
 * hesperus: PhysicsSystem.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PhysicsSystem.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
PhysicsSystem::PhysicsSystem(const BoundsManager_CPtr& boundsManager)
:	m_narrowDetector(boundsManager)
{}

//#################### PUBLIC METHODS ####################
void PhysicsSystem::update(int milliseconds)
{
	typedef std::vector<Contact_CPtr> ContactSet;

	// Step 1:	Update the positions/velocities/etc. of the objects.
	move_objects(milliseconds);

	// Step 2:	Generate all necessary contacts for them (including hard constraints).
	ContactSet contacts;
	detect_contacts(contacts);
	apply_hard_constraints(contacts);

	// Step 3:	Batch the contacts into groups which might mutually interact.
	std::vector<ContactSet> batches = batch_contacts(contacts);

	// Step 4:	Resolve each batch of contacts in turn.
	for(std::vector<ContactSet>::const_iterator it=batches.begin(), iend=batches.end(); it!=iend; ++it)
	{
		resolve_contacts(*it);
	}
}

//#################### PRIVATE METHODS ####################
/**
Apply any hard constraints as additional contacts.

@param contacts	Used to add to the array of contacts which need resolving
*/
void PhysicsSystem::apply_hard_constraints(std::vector<Contact_CPtr>& contacts)
{
	// NYI
	throw 23;
}

/**
Group the contacts into batches which might interact with each other.

@param contacts	The array of contacts which need resolving
@return			The contact batches
*/
std::vector<std::vector<Contact_CPtr> > PhysicsSystem::batch_contacts(const std::vector<Contact_CPtr>& contacts)
{
	// NYI
	throw 23;
}

/**
Perform collision detection and generate any necessary contacts.

@param contacts	Used to add to the array of contacts which need resolving
*/
void PhysicsSystem::detect_contacts(std::vector<Contact_CPtr>& contacts)
{
	// NYI
	throw 23;
}

/**
Accumulate the forces on each physics object and update their positions and velocities appropriately.

@param milliseconds	The length of the time step (in milliseconds)
*/
void PhysicsSystem::move_objects(int milliseconds)
{
	// NYI
	throw 23;
}

/**
Resolve the contacts using the appropriate contact resolver for each pair of colliding objects.

@param contacts	The array of contacts which need resolving
*/
void PhysicsSystem::resolve_contacts(const std::vector<Contact_CPtr>& contacts)
{
	// NYI
	throw 23;
}

}
