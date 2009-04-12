/***
 * hesperus: MovementFunctions.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MovementFunctions.h"

#include <iostream>

#include <source/level/trees/OnionUtil.h>
#include <source/level/trees/TreeUtil.h>
#include <source/math/Constants.h>
#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
bool MovementFunctions::attempt_navmesh_acquisition(const Entity_Ptr& entity, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree,
													const NavMesh_Ptr& navMesh)
{
	ICameraComponent_Ptr camComponent = entity->camera_component();
	INavComponent_Ptr navComponent = entity->nav_component();

	const Vector3d& position = camComponent->camera().position();
	int suggestedNavPoly = navComponent->cur_nav_poly_index();

	int navPolyIndex = find_nav_polygon(position, suggestedNavPoly, polygons, tree, navMesh);
	navComponent->set_cur_nav_poly_index(navPolyIndex);

	return navPolyIndex != -1;
}

/**
Finds the nav polygon in which the specified point resides in the nav mesh, if any.

@param p					The point whose nav polygon we want to find
@param suggestedNavPoly		A suggestion for the result (generally speaking, for a moving entity this would be the last nav polygon we were in)
@param polygons				The collision polygons for the level
@param tree					The onion tree for the level
@param navMesh				The nav mesh in which to find the nav polygon
@return						The index of the nav polygon in which the specified point resides, if any, or -1 otherwise
*/
int MovementFunctions::find_nav_polygon(const Vector3d& p, int suggestedNavPoly, const std::vector<CollisionPolygon_Ptr>& polygons,
										const OnionTree_Ptr& tree, const NavMesh_Ptr& navMesh)
{
	// It's good to be paranoid and do a range check: we might no longer be on the same navigation mesh, for instance.
	if(suggestedNavPoly >= static_cast<int>(navMesh->polygons().size())) suggestedNavPoly = -1;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Step 1:	If there's a suggested nav polygon, check it first.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	int suggestedColPoly = -1;
	if(suggestedNavPoly != -1)
	{
		suggestedColPoly = navMesh->polygons()[suggestedNavPoly]->collision_poly_index();
		if(point_in_polygon(p, *polygons[suggestedColPoly])) return suggestedNavPoly;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Step 2:	Find the other potential collision polygons in which the point could lie.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	int leafIndex = TreeUtil::find_leaf_index(p, tree);
	const std::vector<int>& polyIndices = tree->leaf(leafIndex)->polygon_indices();

	std::vector<int> potentialColPolyIndices;
	for(std::vector<int>::const_iterator it=polyIndices.begin(), iend=polyIndices.end(); it!=iend; ++it)
	{
		// Note: We only add collision polygons which are also nav polygons to the list of potentials.
		if(*it != suggestedColPoly && navMesh->lookup_nav_poly_index(*it) != -1) potentialColPolyIndices.push_back(*it);
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Step 3:	Test each of the polygons to see whether the point's inside it, and return the index of the found nav polygon if so.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	int potentialCount = static_cast<int>(potentialColPolyIndices.size());
	for(int i=0; i<potentialCount; ++i)
	{
		int colPolyIndex = potentialColPolyIndices[i];
		if(point_in_polygon(p, *polygons[colPolyIndex]))
		{
			int navPolyIndex = navMesh->lookup_nav_poly_index(colPolyIndex);
#if 0
			std::cout << "Now in polygon (" << colPolyIndex << ',' << navPolyIndex << ')' << std::endl;
#endif
			return navPolyIndex;
		}
	}

	// If we get here, the point was not in a nav polygon.
	if(suggestedNavPoly != -1)
	{
#if 0
		std::cout << "Lost navmesh at " << p << std::endl;
#endif
	}
	return -1;
}

void MovementFunctions::move_with_navmesh(const Entity_Ptr& entity, const Vector3d& dir, double speed, const std::vector<CollisionPolygon_Ptr>& polygons,
										  const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds)
{
	ICollisionComponent_Ptr colComponent = entity->collision_component();
	INavComponent_Ptr navComponent = entity->nav_component();

	Move move;
	move.dir = dir;
	move.mapIndex = colComponent->aabb_indices()[colComponent->pose()];
	move.timeRemaining = milliseconds / 1000.0;

	NavMesh_Ptr navMesh = navDatasets[move.mapIndex]->nav_mesh();

	double oldTimeRemaining;
	do
	{
		oldTimeRemaining = move.timeRemaining;

		if(navComponent->cur_traversal()) do_traverse_move(entity, move, speed /* FIXME: Select the appropriate speed here */, polygons, navMesh);
		if(move.timeRemaining == 0) break;

		if(attempt_navmesh_acquisition(entity, polygons, tree, navMesh)) do_navmesh_move(entity, move, speed, polygons, tree, navMesh);
		else do_direct_move(entity, move, speed, tree);
	} while(move.timeRemaining > 0 && oldTimeRemaining - move.timeRemaining > 0.0001);
}

/**
@return	true, if a collision occurred, or false otherwise
*/
bool MovementFunctions::single_move_without_navmesh(const Entity_Ptr& entity, const Vector3d& dir, double speed, const OnionTree_Ptr& tree, int milliseconds)
{
	// FIXME: The bool return here is unintuitive and should be replaced with something more sensible.

	ICollisionComponent_Ptr colComponent = entity->collision_component();
	INavComponent_Ptr navComponent = entity->nav_component();

	// Check to make sure we're not currently traversing a link: don't let the entity be moved if we are.
	if(navComponent->cur_traversal()) return true;

	Move move;
	move.dir = dir;
	move.mapIndex = colComponent->aabb_indices()[colComponent->pose()];
	move.timeRemaining = milliseconds / 1000.0;

	return do_direct_move(entity, move, speed, tree);
}

//#################### PRIVATE METHODS ####################
/**
@return	true, if a collision occurred, or false otherwise
*/
bool MovementFunctions::do_direct_move(const Entity_Ptr& entity, Move& move, double speed, const OnionTree_Ptr& tree)
{
	bool collisionOccurred = false;

	ICameraComponent_Ptr camComponent = entity->camera_component();
	ICollisionComponent_Ptr colComponent = entity->collision_component();

	Vector3d source = camComponent->camera().position();
	Vector3d dest = source + move.dir * speed * move.timeRemaining;

	// Check the ray against the tree.
	OnionUtil::Transition transition = OnionUtil::find_first_transition(move.mapIndex, source, dest, tree);
	switch(transition.classifier)
	{
		case OnionUtil::RAY_EMPTY:
		{
			// It's perfectly fine to let the entity move along this ray, as it doesn't intersect a wall.
			break;
		}
		case OnionUtil::RAY_SOLID:
		{
			// We were on a wall (i.e. coplanar to it) prior to the move - prevent any further moves into the wall,
			// and update the move direction to allow sliding along the wall instead.
			update_move_direction_for_sliding(entity, move);
			move.timeRemaining -= 0.001;	// make this cost 1ms of time (otherwise the calling function will think we got stuck)
			return true;
		}
		case OnionUtil::RAY_TRANSITION_ES:
		{
			// Stop the entity going into a wall.
			dest = *transition.location;
			collisionOccurred = true;

			// Record this as the latest transition.
			colComponent->update_recent_transitions(OnionUtil::Transition_Ptr(new OnionUtil::Transition(transition)));

			// Update the move direction to allow sliding.
			update_move_direction_for_sliding(entity, move);

			break;
		}
		case OnionUtil::RAY_TRANSITION_SE:
		{
			// This should never happen (since entities can't move into walls), but better let the entity back into
			// the world if it does happen.
			break;
		}
	}

	camComponent->camera().set_position(dest);

	// Update the time remaining.
	double moveLength = source.distance(dest);
	double timeTaken = moveLength / speed;
	move.timeRemaining -= timeTaken;

	return collisionOccurred;
}

void MovementFunctions::do_navmesh_move(const Entity_Ptr& entity, Move& move, double speed, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree,
										const NavMesh_Ptr& navMesh)
{
	// Step 1:		Project the movement vector onto the plane of the current nav polygon.

	INavComponent_Ptr navComponent = entity->nav_component();
	int curNavPolyIndex = navComponent->cur_nav_poly_index();
	const NavPolygon& navPoly = *navMesh->polygons()[curNavPolyIndex];
	int curColPolyIndex = navPoly.collision_poly_index();
	const CollisionPolygon& curPoly = *polygons[curColPolyIndex];
	Plane plane = make_plane(curPoly);
	move.dir = project_vector_onto_plane(move.dir, plane);
	if(move.dir.length_squared() > SMALL_EPSILON*SMALL_EPSILON) move.dir.normalize();

	// Step 2:		Check whether the new movement vector goes through the influence zone of any of the out navlinks.

	ICameraComponent_Ptr camComponent = entity->camera_component();

	Vector3d source = camComponent->camera().position();
	Vector3d dest = source + move.dir * speed * move.timeRemaining;

	Vector3d_Ptr hit;
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
			camComponent->camera().set_position(dest);
			move.timeRemaining = 0;
		}
		else
		{
			do_direct_move(entity, move, speed, tree);
		}

		return;
	}

	// Step 3.b:	If the new movement vector hits a navlink, move the point at which it first enters the influence zone,
	//				and reduce the time remaining appropriately. Then, initiate the link traversal.

	// Move the entity to the link entrance point.
	camComponent->camera().set_position(*hit);

	// Update the time remaining.
	double moveLength = source.distance(*hit);
	double timeTaken = moveLength / speed;
	move.timeRemaining -= timeTaken;

	// Initiate the link traversal.
	INavComponent::Traversal_Ptr traversal(new INavComponent::Traversal(hitNavlink, *hit, 0));
	navComponent->set_cur_traversal(traversal);
}

void MovementFunctions::do_traverse_move(const Entity_Ptr& entity, Move& move, double speed, const std::vector<CollisionPolygon_Ptr>& polygons, const NavMesh_Ptr& navMesh)
{
	ICameraComponent_Ptr camComponent = entity->camera_component();
	INavComponent_Ptr navComponent = entity->nav_component();

	INavComponent::Traversal_Ptr traversal = navComponent->cur_traversal();
	if(!traversal) return;

	NavLink_Ptr link = navMesh->links()[traversal->linkIndex];
	double remaining = 1 - traversal->t;													// % of link remaining
	double remainingTraversalTime = remaining * link->traversal_time(speed);				// time to traverse remainder
	double availableTraversalTime = std::min(remainingTraversalTime, move.timeRemaining);	// time to spend traversing

	if(availableTraversalTime >= remainingTraversalTime)
	{
		// Finish traversing the link:

		// Update the current nav polygon and clear the current traversal.
		navComponent->set_cur_nav_poly_index(link->dest_poly());
		navComponent->set_cur_traversal(INavComponent::Traversal_Ptr());

		// Move to an exit point on the link.
		Vector3d dest = link->traverse(traversal->source, 1);
		camComponent->camera().set_position(dest);
		move.timeRemaining -= remainingTraversalTime;
		
#if 0
		int colPolyIndex = navMesh->polygons()[link->dest_poly()]->collision_poly_index();
		std::cout << "Linked to polygon (" << colPolyIndex << ',' << link->dest_poly() << ')' << std::endl;
#endif

		// Move the entity very slightly away from the navlink exit: this is a hack to prevent link loops.
		int destColPolyIndex = navMesh->polygons()[link->dest_poly()]->collision_poly_index();
		const CollisionPolygon& destPoly = *polygons[destColPolyIndex];
		Plane destPlane = make_plane(destPoly);
		Vector3d destDir = project_vector_onto_plane(move.dir, destPlane);
		dest += destDir * 0.001;
		if(point_in_polygon(dest, destPoly)) camComponent->camera().set_position(dest);
	}
	else
	{
		// Work out how much further we've progressed and update the traversal field accordingly.
		double deltaT = (availableTraversalTime / remainingTraversalTime) * remaining;
		INavComponent::Traversal_Ptr newTraversal(new INavComponent::Traversal(traversal->linkIndex, traversal->source, traversal->t + deltaT));
		navComponent->set_cur_traversal(newTraversal);

		// Move further along the link.
		Vector3d dest = link->traverse(newTraversal->source, newTraversal->t);
		camComponent->camera().set_position(dest);
		move.timeRemaining = 0;
	}
}

void MovementFunctions::update_move_direction_for_sliding(const Entity_Ptr& entity, Move& move)
{
	// Update the move direction to be along the wall (to allow sliding). To do this, we remove the
	// component of the movement which is normal to the wall. To find the wall we're on, we look at
	// all the 'recent' transitions and choose one which we're trying to walk behind.

	const Vector3d& source = entity->camera_component()->camera().position();
	Vector3d dummyDest = source + move.dir;
	const std::list<OnionUtil::Transition_Ptr>& recentTransitions = entity->collision_component()->recent_transitions();
	for(std::list<OnionUtil::Transition_Ptr>::const_iterator it=recentTransitions.begin(), iend=recentTransitions.end(); it!=iend; ++it)
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
