/***
 * hesperus: NarrowPhaseCollisionDetector.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NarrowPhaseCollisionDetector.h"

#include <cmath>

#include <boost/tuple/tuple.hpp>

#include <source/exceptions/Exception.h>
#include <source/level/collisions/BoundsManager.h>
#include <source/level/trees/OnionUtil.h>
#include <source/math/Constants.h>
#include <source/math/geom/Plane.h>
#include "BoundsSupportMapping.h"
#include "MinkDiffSupportMapping.h"
#include "NormalPhysicsObject.h"
#include "SegmentSupportMapping.h"
#include "SweptSupportMapping.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
NarrowPhaseCollisionDetector::NarrowPhaseCollisionDetector(const BoundsManager_CPtr& boundsManager)
:	m_boundsManager(boundsManager)
{}

//#################### PUBLIC METHODS ####################
boost::optional<Contact>
NarrowPhaseCollisionDetector::object_vs_object(PhysicsObject& objectA, PhysicsObject& objectB) const
{
	/*
	The algorithm used is known as Minkowski Portal Refinement (MPR), or XenoCollide,
	which is described in an article in Game Programming Gems 7 (starting on p.165).

	The conversion to a swept collision test is based on a post by Gary Snethen on
	the XenoCollide forums.
	*/

	SupportMapping_CPtr mapping;
	Vector3d interiorPoint;
	boost::tie(mapping, interiorPoint) = construct_support_mapping(objectA, objectB);
	boost::optional<Contact> contact = xeno_collide(mapping, interiorPoint);
	return convert_to_world_contact(contact, objectA, objectB);
}

boost::optional<Contact>
NarrowPhaseCollisionDetector::object_vs_world(NormalPhysicsObject& object, const OnionTree_CPtr& tree) const
{
	// If the object hasn't moved, no contact should be generated (we justifiably assume the object didn't
	// start off embedded in a wall).
	if(!object.previous_position()) return boost::none;
	const Vector3d& pos = object.position();
	const Vector3d& previousPos = *object.previous_position();
	if((pos - previousPos).length() < SMALL_EPSILON) return boost::none;

	int mapIndex = m_boundsManager->lookup_bounds_index(object.bounds_group(), object.posture());
	OnionUtil::Transition transition = OnionUtil::find_first_transition(mapIndex, previousPos, pos, tree);

	switch(transition.classifier)
	{
		case OnionUtil::RAY_EMPTY:
		{
			// The object's movement lies entirely in empty space, so there's no collision.
			return boost::none;
		}
		case OnionUtil::RAY_SOLID:
		{
			// NYI
			throw 23;
		}
		case OnionUtil::RAY_TRANSITION_ES:
		{
			const Vector3d& contactPoint = *transition.location;
			const Vector3d& contactNormal = transition.plane->normal();
			Vector3d penetration = contactPoint - pos;
			double penetrationDepth = penetration.dot(contactNormal);	// note that |contactNormal| = 1

			// Calculate the intersection time (as a number in [0,1]).
			double contactDistSquared = (contactPoint - previousPos).length_squared();
			double moveDistSquared = (pos - previousPos).length_squared();
			assert(moveDistSquared > 0);
			double time = sqrt(contactDistSquared / moveDistSquared);

			return Contact(contactPoint, contactNormal, penetrationDepth, time, object);
		}
		case OnionUtil::RAY_TRANSITION_SE:
		{
			// This should never happen (since objects can't move into walls), but better let the object back into
			// the world if it does happen.
			return boost::none;
		}
		default:
		{
			throw Exception("This will never happen as the case statement is exhaustive");
		}
	}
}

//#################### PRIVATE METHODS ####################
std::pair<SupportMapping_CPtr,Vector3d>
NarrowPhaseCollisionDetector::construct_support_mapping(PhysicsObject& objectA, PhysicsObject& objectB) const
{
	// Construct the support mapping in the reference frame of A. (In that frame, A is centred at the origin.)
	// We'll be colliding a relative, swept version of B (called S) against a stationary A and then transforming
	// the result back into world space.

	Vector3d aPos0 = objectA.previous_position() ? *objectA.previous_position() : objectA.position();
	Vector3d aPos1 = objectA.position();
	Vector3d bPos0 = objectB.previous_position() ? *objectB.previous_position() : objectB.position();
	Vector3d bPos1 = objectB.position();

	Vector3d bRelPos0 = bPos0 - aPos0;
	Vector3d bRelPos1 = bPos1 - aPos1;
	Vector3d bRelDir = bRelPos1 - bRelPos0;

	// Construct the support mapping for the stationary A.
	Bounds_CPtr aBounds = objectA.bounds(m_boundsManager);
	SupportMapping_CPtr mappingA(new BoundsSupportMapping(aBounds));

	// Construct the support mapping for S (the relative, swept B).
	Bounds_CPtr bBounds = objectB.bounds(m_boundsManager);
	SupportMapping_CPtr mappingB(new BoundsSupportMapping(bBounds));
	SupportMapping_CPtr mappingL(new SegmentSupportMapping(bRelPos0, bRelPos1));
	SupportMapping_CPtr mappingS(new SweptSupportMapping(mappingB, mappingL));

	// Construct the support mapping for their Minkowski difference (S - A).
	SupportMapping_CPtr mapping(new MinkDiffSupportMapping(mappingS, mappingA));

	// Find a point interior to S - A. Note that the midpoint of B's relative movement
	// vector will do because it's definitely inside S (which is the relative sweep of
	// B as it moves), whilst A is centred at the origin and symmetrical about it, so
	// subtracting A won't move the overall shape.
	Vector3d interiorPoint = (bRelPos0 + bRelPos1) / 2;

	// Ensure that the interior point isn't too near the origin. If it is, any point
	// right next to the origin can be substituted instead.
	if(interiorPoint.length_squared() < EPSILON*EPSILON) interiorPoint = Vector3d(EPSILON, 0, 0);

	return std::make_pair(mapping, interiorPoint);
}

boost::optional<Contact>
NarrowPhaseCollisionDetector::convert_to_world_contact(const boost::optional<Contact>& relativeContact,
													   PhysicsObject& objectA, PhysicsObject& objectB) const
{
	// NYI
	throw 23;
}

boost::optional<Contact>
NarrowPhaseCollisionDetector::xeno_collide(const SupportMapping_CPtr& mapping, const Vector3d& v0)
{
	MinkPortal portal;
	Vector3d& v1 = portal.v1;
	Vector3d& v2 = portal.v2;
	Vector3d& v3 = portal.v3;

	// Find the support point in the direction of the origin ray 0 - v0 = -v0.
	Vector3d n = (-v0).normalize();		// note: v0 is guaranteed to be non-zero by construction
	v1 = (*mapping)(n);

	if(n.dot(v1) <= 0)
	{
		// Special Case: The origin doesn't lie on the near side of the support plane containing v1 -> MISS
		// (Note that the plane equation is given by n.x - n.v1 = 0, so if n.v1 <= 0 then n.x - n.v1 >= 0,
		// i.e. the origin is in front of, or on, a support plane pointing away from the shape.)
		return boost::none;
	}

	// Find the support point along the normal of the plane containing the origin, v0 and v1.
	n = v1.cross(v0);

	if(n.length_squared() < EPSILON*EPSILON)
	{
		// Special Case: v1 lies on the line joining the origin and v0.
		// Since it's a point on the surface of the Minkowski difference shape,
		// we can use it to test directly whether or not the origin is within
		// the shape.
		Vector3d v01 = v1 - v0;
		if(v0.length_squared() < v01.length_squared())
		{
			// v0 is closer to the origin than it is to v1, so (by the geometry of the situation) the origin
			// must lie between v0 and v1 and thus be inside the Minkowski difference shape -> HIT

			// NYI
			throw 23;
		}
		else return boost::none;
	}

	n.normalize();	// note: if we get here, n is non-zero
	v2 = (*mapping)(n);

	// Find the support point along the normal of the plane containing v0, v1 and v2.
	// TODO

	// NYI
	throw 23;
}

}
