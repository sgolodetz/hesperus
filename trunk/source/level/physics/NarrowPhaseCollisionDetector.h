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
	void construct_support_mappings(const PhysicsObject& objectA, const PhysicsObject& objectB, SupportMapping_CPtr& mapping, SupportMapping_CPtr& mappingA, SupportMapping_CPtr& mappingS, Vector3d& interiorPoint, double& distanceMoved) const;
	boost::optional<Contact> convert_to_world_contact(const boost::optional<Contact>& relativeContact) const;
	static Contact make_contact(const Vector3d& v0, const SupportMapping_CPtr& mappingA, const SupportMapping_CPtr& mappingS, double distanceMoved, PhysicsObject& objectA, PhysicsObject& objectB);
	static boost::optional<Contact> xeno_collide(PhysicsObject& objectA, PhysicsObject& objectB, const SupportMapping_CPtr& mapping, const SupportMapping_CPtr& mappingA, const SupportMapping_CPtr& mappingS, const Vector3d& v0, double distanceMoved);
};

}

#endif
