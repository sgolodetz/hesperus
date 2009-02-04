/***
 * hesperus: MovementFunctions.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MovementFunctions.h"

#include <iostream>

#include <source/math/Constants.h>
#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
void MovementFunctions::move_with_navmesh(const Entity_Ptr& entity, const Vector3d& dir, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree,
										  const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds)
{
	ICollisionComponent_Ptr colComponent = entity->collision_component();

	Move move;
	move.dir = dir;
	move.mapIndex = colComponent->aabb_indices()[colComponent->pose()];
	move.timeRemaining = milliseconds / 1000.0;

	NavMesh_Ptr navMesh = navDatasets[move.mapIndex]->nav_mesh();

	double oldTimeRemaining;
	do
	{
		oldTimeRemaining = move.timeRemaining;
		if(attempt_navmesh_acquisition(entity, polygons, tree, navMesh)) do_navmesh_move(entity, move, polygons, tree, navMesh);
		else do_direct_move(entity, move, tree);
	} while(move.timeRemaining > 0 && oldTimeRemaining - move.timeRemaining > 0.0001);
}

#if 0
void MovementFunctions::move_without_navmesh(const Entity_Ptr& entity, const Vector3d& dir, const OnionTree_Ptr& tree, int milliseconds)
{
	ICollisionComponent_Ptr colComponent = entity->collision_component();

	Move move;
	move.dir = dir;
	move.mapIndex = colComponent->aabb_indices()[colComponent->pose()];
	move.timeRemaining = milliseconds / 1000.0;

	double oldTimeRemaining;
	do
	{
		oldTimeRemaining = move.timeRemaining;
		do_direct_move(entity, move, tree);
	} while(move.timeRemaining > 0 && oldTimeRemaining - move.timeRemaining > 0.0001);
}
#endif

/**
@return	true, if a collision occurred, or false otherwise
*/
bool MovementFunctions::single_move_without_navmesh(const Entity_Ptr& entity, const Vector3d& dir,
													const OnionTree_Ptr& tree, int milliseconds)
{
	ICollisionComponent_Ptr colComponent = entity->collision_component();

	Move move;
	move.dir = dir;
	move.mapIndex = colComponent->aabb_indices()[colComponent->pose()];
	move.timeRemaining = milliseconds / 1000.0;

	return do_direct_move(entity, move, tree);
}

//#################### PRIVATE METHODS ####################
bool MovementFunctions::attempt_navmesh_acquisition(const Entity_Ptr& entity, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree,
													const NavMesh_Ptr& navMesh)
{
	ICameraComponent_Ptr camComponent = entity->camera_component();
	const Vector3d& position = camComponent->camera().position();

	// Step 1:	If this entity's nav component contains a 'current' nav poly for the entity (which amounts to a suggestion as to where it
	//			might still be), check it first.

	int suggestedColPoly = -1;
	INavComponent_Ptr navComponent = entity->nav_component();
	int suggestedNavPoly = navComponent->cur_nav_poly_index();

	// It's good to be paranoid and do a range check: we might no longer be on the same navigation mesh, for instance.
	if(suggestedNavPoly >= static_cast<int>(navMesh->polygons().size()))
	{
		navComponent->set_cur_nav_poly_index(-1);
		suggestedNavPoly = -1;
	}

	if(suggestedNavPoly != -1)
	{
		suggestedColPoly = navMesh->polygons()[suggestedNavPoly]->collision_poly_index();
		if(point_in_polygon(position, *polygons[suggestedColPoly])) return true;
	}

	// Step 2:	Find the other potential collision polygons this entity could be in.

	int leafIndex = tree->find_leaf_index(position);
	const std::vector<int>& polyIndices = tree->leaf(leafIndex)->polygon_indices();

	std::vector<int> potentialColPolyIndices;
	for(std::vector<int>::const_iterator it=polyIndices.begin(), iend=polyIndices.end(); it!=iend; ++it)
	{
		// Note: We only add collision polygons which are also nav polygons to the list of potentials.
		if(*it != suggestedColPoly && navMesh->lookup_nav_poly_index(*it) != -1) potentialColPolyIndices.push_back(*it);
	}

	// Step 3:	Test each of the polygons to see whether our current position's inside it, and acquire the navmesh if so.

	int potentialCount = static_cast<int>(potentialColPolyIndices.size());
	for(int i=0; i<potentialCount; ++i)
	{
		int colPolyIndex = potentialColPolyIndices[i];
		if(point_in_polygon(position, *polygons[colPolyIndex]))
		{
			int navPolyIndex = navMesh->lookup_nav_poly_index(colPolyIndex);
			navComponent->set_cur_nav_poly_index(navPolyIndex);
#if 0
			std::cout << "Now in polygon (" << colPolyIndex << ',' << navPolyIndex << ')' << std::endl;
#endif
			return true;
		}
	}

	// If we get here, we were unable to acquire the navmesh.
	if(suggestedNavPoly != -1)
	{
		navComponent->set_cur_nav_poly_index(-1);
#if 0
		std::cout << "Lost navmesh at " << position << std::endl;
#endif
	}
	return false;
}

/**
@return	true, if a collision occurred, or false otherwise
*/
bool MovementFunctions::do_direct_move(const Entity_Ptr& entity, Move& move, const OnionTree_Ptr& tree)
{
	bool collisionOccurred = false;

	// FIXME: Walking speed will eventually be a property of the entity.
	const double WALK_SPEED = 5.0;	// in units/s

	ICameraComponent_Ptr camComponent = entity->camera_component();
	ICollisionComponent_Ptr colComponent = entity->collision_component();

	Vector3d source = camComponent->camera().position();
	Vector3d dest = source + move.dir * WALK_SPEED * move.timeRemaining;

	// Check the ray against the tree.
	OnionTree::Transition transition = tree->find_first_transition(move.mapIndex, source, dest);
	switch(transition.classifier)
	{
		case OnionTree::RAY_EMPTY:
		{
			// It's perfectly fine to let the entity move along this ray, as it doesn't intersect a wall.
			break;
		}
		case OnionTree::RAY_SOLID:
		{
			// We were on a wall (i.e. coplanar to it) prior to the move - prevent any further moves into the wall,
			// and update the move direction to allow sliding along the wall instead.
			update_move_direction_for_sliding(entity, move);
			move.timeRemaining -= 0.001;	// make this cost 1ms of time (otherwise the calling function will think we got stuck)
			return true;
		}
		case OnionTree::RAY_TRANSITION_ES:
		{
			// Stop the entity going into a wall.
			dest = *transition.location;
			collisionOccurred = true;

			// Record this as the latest transition.
			colComponent->update_last_transition(OnionTree::Transition_Ptr(new OnionTree::Transition(transition)));

			// Update the move direction to allow sliding.
			update_move_direction_for_sliding(entity, move);

			break;
		}
		case OnionTree::RAY_TRANSITION_SE:
		{
			// This should never happen (since entities can't move into walls), but better let the entity back into
			// the world if it does happen.
			break;
		}
	}

	camComponent->camera().set_position(dest);

	// Update the time remaining.
	double moveLength = source.distance(dest);
	double timeTaken = moveLength / WALK_SPEED;
	move.timeRemaining -= timeTaken;

	return collisionOccurred;
}

void MovementFunctions::do_navmesh_move(const Entity_Ptr& entity, Move& move, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree,
										const NavMesh_Ptr& navMesh)
{
	// Step 1:		Project the movement vector onto the plane of the current nav polygon.

	INavComponent_Ptr navComponent = entity->nav_component();
	int curNavPolyIndex = navComponent->cur_nav_poly_index();
	const NavPolygon& navPoly = *navMesh->polygons()[curNavPolyIndex];
	int curColPolyIndex = navPoly.collision_poly_index();
	const CollisionPolygon& curPoly = *polygons[curColPolyIndex];
	Plane plane = make_plane(curPoly);
	Vector3d dir = project_vector_onto_plane(move.dir, plane);

	// Step 2:		Check whether the new movement vector goes through the influence zone of any of the out navlinks.

	// FIXME: Walking speed will eventually be a property of the entity.
	const double WALK_SPEED = 5.0;	// in units/s

	ICameraComponent_Ptr camComponent = entity->camera_component();

	Vector3d source = camComponent->camera().position();
	Vector3d dest = source + dir * WALK_SPEED * move.timeRemaining;

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
			do_direct_move(entity, move, tree);
		}

		return;
	}

	// Step 3.b:	If the new movement vector hits a navlink, move to the point at which it first enters the influence zone,
	//				and reduce the time remaining appropriately. Next, calculate the time (for this entity) to traverse the
	//				navlink completely. Traverse it as much as we are able in the time remaining and decrease the time
	//				remaining appropriately. If we traversed it completely, update the current nav polygon.

	camComponent->camera().set_position(*hit);

	// Update the time remaining.
	double moveLength = source.distance(*hit);
	double timeTaken = moveLength / WALK_SPEED;
	move.timeRemaining -= timeTaken;

	NavLink_Ptr link = navMesh->links()[hitNavlink];
	double traversalTime = link->traversal_time(WALK_SPEED);
	double availableTraversalTime = std::max(traversalTime, move.timeRemaining);
	double t = traversalTime > 0 ? availableTraversalTime / traversalTime : 1;
	dest = link->traverse(*hit, t);

	camComponent->camera().set_position(dest);
	move.timeRemaining -= std::min(traversalTime, availableTraversalTime);

	if(fabs(t - 1) < SMALL_EPSILON)
	{
		navComponent->set_cur_nav_poly_index(link->dest_poly());
		
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
		// TODO: For things like ladders, we'll need to make a note that we're in the middle of a traversal for future frames.
	}
}

void MovementFunctions::update_move_direction_for_sliding(const Entity_Ptr& entity, Move& move)
{
	// Update the move direction to be along the wall (to allow sliding). To do this, we remove the
	// component of the movement which is normal to the wall.

	ICollisionComponent_Ptr colComponent = entity->collision_component();
	const OnionTree::Transition_Ptr& transition = colComponent->last_transition();

	if(transition)
	{
		move.dir = project_vector_onto_plane(move.dir, *transition->plane);
		if(move.dir.length() > SMALL_EPSILON) move.dir.normalize();
		else move.timeRemaining = 0;
	}
}

}
