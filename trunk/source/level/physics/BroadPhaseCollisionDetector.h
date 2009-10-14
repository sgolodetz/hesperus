/***
 * hesperus: BroadPhaseCollisionDetector.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BROADPHASECOLLISIONDETECTOR
#define H_HESP_BROADPHASECOLLISIONDETECTOR

#include <utility>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class Contact> Contact_CPtr;
typedef shared_ptr<class PhysicsObject> PhysicsObject_Ptr;

class BroadPhaseCollisionDetector
{
	//#################### TYPEDEFS ####################
public:
	typedef std::pair<PhysicsObject_Ptr,PhysicsObject_Ptr> ObjectPair;

	//#################### PUBLIC METHODS ####################
public:
	void add_object(const PhysicsObject_Ptr& object);
	std::vector<ObjectPair> potential_collisions() const;
	void reset();
};

}

#endif
