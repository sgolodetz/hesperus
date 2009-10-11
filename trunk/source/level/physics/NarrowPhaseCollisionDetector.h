/***
 * hesperus: NarrowPhaseCollisionDetector.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NARROWPHASECOLLISIONDETECTOR
#define H_HESP_NARROWPHASECOLLISIONDETECTOR

#include "Contact.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class BoundsManager> BoundsManager_CPtr;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;
class NormalPhysicsObject;
class PhysicsObject;
typedef shared_ptr<const class Plane> Plane_CPtr;
typedef shared_ptr<const class SupportMapping> SupportMapping_CPtr;

class NarrowPhaseCollisionDetector
{
	//#################### NESTED CLASSES ####################
private:
	struct MinkPortal
	{
		Vector3d v1, v2, v3;	// the three points that make up the triangular portal
		Plane_CPtr plane;		// a plane through the three points, facing away from the interior point in XenoCollide
	};

	//#################### PRIVATE VARIABLES ####################
private:
	BoundsManager_CPtr m_boundsManager;

	//#################### CONSTRUCTORS ####################
public:
	explicit NarrowPhaseCollisionDetector(const BoundsManager_CPtr& boundsManager);

	//#################### PUBLIC METHODS ####################
public:
	boost::optional<Contact> object_vs_object(PhysicsObject& objectA, PhysicsObject& objectB) const;
	boost::optional<Contact> object_vs_world(NormalPhysicsObject& object, const OnionTree_CPtr& tree) const;

	//#################### PRIVATE METHODS ####################
private:
	std::pair<SupportMapping_CPtr,Vector3d> construct_support_mapping(PhysicsObject& objectA, PhysicsObject& objectB) const;
	boost::optional<Contact> convert_to_world_contact(const boost::optional<Contact>& relativeContact, PhysicsObject& objectA, PhysicsObject& objectB) const;
	static boost::optional<Contact> xeno_collide(const SupportMapping_CPtr& mapping, const Vector3d& v0);
};

}

#endif
