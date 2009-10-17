/***
 * hesperus: MoveFunctions.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MoveFunctions.h"

#include <iostream>

#include <source/level/bounds/BoundsManager.h>
#include <source/level/nav/NavDataset.h>
#include <source/level/nav/NavLink.h>
#include <source/level/nav/NavMesh.h>
#include <source/level/nav/NavMeshUtil.h>
#include <source/level/nav/NavPolygon.h>
#include <source/level/objects/components/ICmpMeshMovement.h>
#include <source/level/objects/components/ICmpPosition.h>
#include <source/level/objects/components/ICmpSimulation.h>
#include <source/level/trees/TreeUtil.h>
#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
bool MoveFunctions::attempt_navmesh_acquisition(const ObjectID& objectID, ObjectManager *objectManager, const std::vector<CollisionPolygon_Ptr>& polygons,
												const OnionTree_CPtr& tree, const NavMesh_CPtr& navMesh)
{
	ICmpMeshMovement_Ptr cmpMovement = objectManager->get_component(objectID, cmpMovement);		assert(cmpMovement != NULL);
	ICmpPosition_Ptr cmpPosition = objectManager->get_component(objectID, cmpPosition);			assert(cmpPosition != NULL);

	const Vector3d& position = cmpPosition->position();
	int suggestedNavPoly = cmpMovement->cur_nav_poly_index();

	int navPolyIndex = NavMeshUtil::find_nav_polygon(position, suggestedNavPoly, polygons, tree, navMesh);
	cmpMovement->set_cur_nav_poly_index(navPolyIndex);

	return navPolyIndex != -1;
}

void MoveFunctions::move_with_navmesh(const ObjectID& objectID, const ObjectManager_Ptr& objectManager, const Vector3d& dir, double speed,
									  const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const std::vector<NavDataset_Ptr>& navDatasets,
									  int milliseconds)
{
	ICmpMeshMovement_Ptr cmpMovement = objectManager->get_component(objectID, cmpMovement);		assert(cmpMovement != NULL);
	ICmpSimulation_Ptr cmpSimulation = objectManager->get_component(objectID, cmpSimulation);	assert(cmpSimulation != NULL);

	Move move;
	move.dir = dir;
	move.mapIndex = objectManager->bounds_manager()->lookup_bounds_index(cmpSimulation->bounds_group(), cmpSimulation->posture());
	move.timeRemaining = milliseconds / 1000.0;

	NavMesh_Ptr navMesh = navDatasets[move.mapIndex]->nav_mesh();

	double oldTimeRemaining;
	do
	{
		oldTimeRemaining = move.timeRemaining;

		if(cmpMovement->cur_traversal()) do_traverse_move(objectID, objectManager, move, speed /* FIXME: Select the appropriate speed here */, polygons, navMesh);
		if(move.timeRemaining == 0) break;

		if(attempt_navmesh_acquisition(objectID, objectManager.get(), polygons, tree, navMesh)) do_navmesh_move(objectID, objectManager, move, speed, polygons, tree, navMesh);
		else do_direct_move(objectID, objectManager, move, speed, tree);
	} while(move.timeRemaining > 0 && oldTimeRemaining - move.timeRemaining > 0.0001);
}

/**
@return	true, if a collision occurred, or false otherwise
*/
bool MoveFunctions::single_move_without_navmesh(const ObjectID& objectID, const ObjectManager_Ptr& objectManager, const Vector3d& dir, double speed,
												const OnionTree_CPtr& tree, int milliseconds)
{
	// FIXME: The bool return here is unintuitive and should be replaced with something more sensible.

	ICmpMeshMovement_Ptr cmpMovement = objectManager->get_component(objectID, cmpMovement);
	ICmpSimulation_Ptr cmpSimulation = objectManager->get_component(objectID, cmpSimulation);	assert(cmpSimulation != NULL);

	// Check to make sure we're not currently traversing a link: don't let the object be moved if we are.
	if(cmpMovement && cmpMovement->cur_traversal()) return true;

	Move move;
	move.dir = dir;
	move.mapIndex = objectManager->bounds_manager()->lookup_bounds_index(cmpSimulation->bounds_group(), cmpSimulation->posture());
	move.timeRemaining = milliseconds / 1000.0;

	return do_direct_move(objectID, objectManager, move, speed, tree);
}

//#################### PRIVATE METHODS ####################
/**
@return	true, if a collision occurred, or false otherwise
*/
bool MoveFunctions::do_direct_move(const ObjectID& objectID, const ObjectManager_Ptr& objectManager, Move& move, double speed, const OnionTree_CPtr& tree)
{
	bool collisionOccurred = false;

	ICmpDirectMovement_Ptr cmpMovement = objectManager->get_component(objectID, cmpMovement);	assert(cmpMovement != NULL);
	ICmpPosition_Ptr cmpPosition = objectManager->get_component(objectID, cmpPosition);			assert(cmpPosition != NULL);

	Vector3d source = cmpPosition->position();
	Vector3d dest = source + move.dir * speed * move.timeRemaining;

	// Check the ray against the tree.
	OnionUtil::Transition transition = OnionUtil::find_first_transition(move.mapIndex, source, dest, tree);
	switch(transition.classifier)
	{
		case OnionUtil::RAY_EMPTY:
		{
			// It's perfectly fine to let the object move along this ray, as it doesn't intersect a wall.
			break;
		}
		case OnionUtil::RAY_SOLID:
		{
			// We were on a wall (i.e. coplanar to it) prior to the move - prevent any further moves into the wall,
			// and update the move direction to allow sliding along the wall instead.
			update_move_direction_for_sliding(objectID, objectManager, move);
			move.timeRemaining -= 0.001;	// make this cost 1ms of time (otherwise the calling function will think we got stuck)
			return true;
		}
		case OnionUtil::RAY_TRANSITION_ES:
		{
			// Stop the object going into a wall.
			dest = *transition.location;
			collisionOccurred = true;

			// Record this as the latest transition.
			cmpMovement->update_recent_transitions(OnionUtil::Transition_Ptr(new OnionUtil::Transition(transition)));

			// Update the move direction to allow sliding.
			update_move_direction_for_sliding(objectID, objectManager, move);

			break;
		}
		case OnionUtil::RAY_TRANSITION_SE:
		{
			// This should never happen (since objects can't move into walls), but better let the object back into
			// the world if it does happen.
			break;
		}
	}

	cmpPosition->set_position(dest);

	// Update the time remaining.
	double moveLength = source.distance(dest);
	double timeTaken = moveLength / speed;
	move.timeRemaining -= timeTaken;

	return collisionOccurred;
}

void MoveFunctions::do_navmesh_move(const ObjectID& objectID, const ObjectManager_Ptr& objectManager, Move& move, double speed,
									const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavMesh_CPtr& navMesh)
{
	ICmpMeshMovement_Ptr cmpMovement = objectManager->get_component(objectID, cmpMovement);		assert(cmpMovement != NULL);
	ICmpPosition_Ptr cmpPosition = objectManager->get_component(objectID, cmpPosition);			assert(cmpPosition != NULL);

	// Step 1:		Project the movement vector onto the plane of the current nav polygon.

	int curNavPolyIndex = cmpMovement->cur_nav_poly_index();
	const NavPolygon& navPoly = *navMesh->polygons()[curNavPolyIndex];
	int curColPolyIndex = navPoly.collision_poly_index();
	const CollisionPolygon& curPoly = *polygons[curColPolyIndex];
	Plane plane = make_plane(curPoly);
	move.dir = project_vector_onto_plane(move.dir, plane);
	if(move.dir.length_squared() > SMALL_EPSILON*SMALL_EPSILON) move.dir.normalize();

	// Step 2:		Check whether the new movement vector goes through the influence zone of any of the out navlinks.

	Vector3d source = cmpPosition->position();
	Vector3d dest = source + move.dir * speed * move.timeRemaining;

	boost::optional<Vector3d> hit;
	int hitNavlink = -1;
	const std::vector<int>& links = navPoly.out_links();
	for(std::vector<int>::const_iterator it=links.begin(), iend=links.end(); it!=iend; ++it)
	{
		const NavLink_Ptr& link = navMesh->links()[*it];
		hit = link->hit_test(source, dest);

		if(hit)
		{
			hitNavlink = *it;
#if 0
			std::cout << "Hit navlink at " << *hit << ": ";
			link->output(std::cout);
			std::cout << '\n';
#endif
			break;
		}
	}

	// Step 3.a:	If the new movement vector doesn't hit a navlink, check whether the other end of the movement vector is within the current polygon.
	//
	//				-	If yes, move there, set the time remaining to zero and return.
	//
	//				-	If no, do a direct move in the original direction, since we are either leaving the navmesh or hitting a wall.

	if(hitNavlink == -1)
	{
		if(point_in_polygon(dest, curPoly))
		{
			cmpPosition->set_position(dest);
			move.timeRemaining = 0;
		}
		else
		{
			do_direct_move(objectID, objectManager, move, speed, tree);
		}

		return;
	}

	// Step 3.b:	If the new movement vector hits a navlink, move the point at which it first enters the influence zone,
	//				and reduce the time remaining appropriately. Then, initiate the link traversal.

	// Move the object to the link entrance point.
	cmpPosition->set_position(*hit);

	// Update the time remaining.
	double moveLength = source.distance(*hit);
	double timeTaken = moveLength / speed;
	move.timeRemaining -= timeTaken;

	// Initiate the link traversal.
	ICmpMeshMovement::Traversal_Ptr traversal(new ICmpMeshMovement::Traversal(hitNavlink, *hit, 0));
	cmpMovement->set_cur_traversal(traversal);
}

void MoveFunctions::do_traverse_move(const ObjectID& objectID, const ObjectManager_Ptr& objectManager, Move& move, double speed,
									 const std::vector<CollisionPolygon_Ptr>& polygons, const NavMesh_CPtr& navMesh)
{
	ICmpMeshMovement_Ptr cmpMovement = objectManager->get_component(objectID, cmpMovement);		assert(cmpMovement != NULL);
	ICmpPosition_Ptr cmpPosition = objectManager->get_component(objectID, cmpPosition);			assert(cmpPosition != NULL);

	ICmpMeshMovement::Traversal_CPtr traversal = cmpMovement->cur_traversal();
	if(!traversal) return;

	NavLink_Ptr link = navMesh->links()[traversal->linkIndex];
	double remaining = 1 - traversal->t;													// % of link remaining
	double remainingTraversalTime = remaining * link->traversal_time(speed);				// time to traverse remainder
	double availableTraversalTime = std::min(remainingTraversalTime, move.timeRemaining);	// time to spend traversing

	if(availableTraversalTime >= remainingTraversalTime)
	{
		// Finish traversing the link:

		// Update the current nav polygon and clear the current traversal.
		cmpMovement->set_cur_nav_poly_index(link->dest_poly());
		cmpMovement->set_cur_traversal(ICmpMeshMovement::Traversal_Ptr());

		// Move to an exit point on the link.
		Vector3d dest = link->traverse(traversal->source, 1);
		cmpPosition->set_position(dest);
		move.timeRemaining -= remainingTraversalTime;
		
#if 0
		int colPolyIndex = navMesh->polygons()[link->dest_poly()]->collision_poly_index();
		std::cout << "Linked to polygon (" << colPolyIndex << ',' << link->dest_poly() << ')' << std::endl;
#endif

		// Move the object very slightly away from the navlink exit: this is a hack to prevent link loops.
		int destColPolyIndex = navMesh->polygons()[link->dest_poly()]->collision_poly_index();
		const CollisionPolygon& destPoly = *polygons[destColPolyIndex];
		Plane destPlane = make_plane(destPoly);
		Vector3d destDir = project_vector_onto_plane(move.dir, destPlane);
		dest += destDir * 0.001;
		if(point_in_polygon(dest, destPoly)) cmpPosition->set_position(dest);
	}
	else
	{
		// Work out how much further we've progressed and update the traversal field accordingly.
		double deltaT = (availableTraversalTime / remainingTraversalTime) * remaining;
		ICmpMeshMovement::Traversal_Ptr newTraversal(new ICmpMeshMovement::Traversal(traversal->linkIndex, traversal->source, traversal->t + deltaT));
		cmpMovement->set_cur_traversal(newTraversal);

		// Move further along the link.
		Vector3d dest = link->traverse(newTraversal->source, newTraversal->t);
		cmpPosition->set_position(dest);
		move.timeRemaining = 0;
	}
}

void MoveFunctions::update_move_direction_for_sliding(const ObjectID& objectID, const ObjectManager_Ptr& objectManager, Move& move)
{
	// Update the move direction to be along the wall (to allow sliding). To do this, we remove the
	// component of the movement which is normal to the wall. To find the wall we're on, we look at
	// all the 'recent' transitions and choose one which we're trying to walk behind.

	ICmpDirectMovement_Ptr cmpMovement = objectManager->get_component(objectID, cmpMovement);	assert(cmpMovement != NULL);
	ICmpPosition_Ptr cmpPosition = objectManager->get_component(objectID, cmpPosition);			assert(cmpPosition != NULL);
	
	const Vector3d& source = cmpPosition->position();
	Vector3d dummyDest = source + move.dir;
	const std::list<OnionUtil::Transition_CPtr>& recentTransitions = cmpMovement->recent_transitions();
	for(std::list<OnionUtil::Transition_CPtr>::const_iterator it=recentTransitions.begin(), iend=recentTransitions.end(); it!=iend; ++it)
	{
		if(classify_point_against_plane(dummyDest, *(*it)->plane) == CP_BACK)
		{
			move.dir = project_vector_onto_plane(move.dir, *(*it)->plane);
			if(move.dir.length() > SMALL_EPSILON) move.dir.normalize();
			else move.timeRemaining = 0;

			break;
		}
	}
}

}
