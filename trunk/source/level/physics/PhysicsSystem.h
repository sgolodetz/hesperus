/***
 * hesperus: PhysicsSystem.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PHYSICSSYSTEM
#define H_HESP_PHYSICSSYSTEM

#include <vector>

#include "Contact.h"
#include "NarrowPhaseCollisionDetector.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class BoundsManager> BoundsManager_CPtr;
typedef shared_ptr<const class Contact> Contact_CPtr;

class PhysicsSystem
{
	//#################### PRIVATE VARIABLES ####################
private:
	NarrowPhaseCollisionDetector m_narrowDetector;
	// TODO: Broad phase collision detection

	//#################### CONSTRUCTORS ####################
public:
	explicit PhysicsSystem(const BoundsManager_CPtr& boundsManager);

	//#################### PUBLIC METHODS ####################
public:
	void update(int milliseconds);

	//#################### PRIVATE METHODS ####################
private:
	void apply_hard_constraints(std::vector<Contact_CPtr>& contacts);
	std::vector<std::vector<Contact_CPtr> > batch_contacts(const std::vector<Contact_CPtr>& contacts);
	void detect_contacts(std::vector<Contact_CPtr>& contacts);
	void move_objects(int milliseconds);
	void resolve_contacts(const std::vector<Contact_CPtr>& contacts);
};

}

#endif
