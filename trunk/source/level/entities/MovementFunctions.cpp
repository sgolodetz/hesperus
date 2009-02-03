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
	} while(move.timeRemaining > 0.0005 && oldTimeRemaining - move.timeRemaining > 0.0001);
}

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
	} while(move.timeRemaining > 0.0005 && oldTimeRemaining - move.timeRemaining > 0.0001);
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
	return false;
}

void MovementFunctions::do_direct_move(const Entity_Ptr& entity, Move& move, const OnionTree_Ptr& tree)
{
	// FIXME: Walking speed will eventually be a property of the entity.
	const double WALK_SPEED = 5.0;	// in units/s

	ICameraComponent_Ptr camComponent = entity->camera_component();
	ICollisionComponent_Ptr colComponent = entity->collision_component();

	const Vector3d& source = camComponent->camera().position();
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
			return;
		}
		case OnionTree::RAY_TRANSITION_ES:
		{
			// Stop the entity going into a wall.
			dest = *transition.location;

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

	// Update the time remaining.
	double moveLength = source.distance(dest);
	double timeTaken = moveLength / WALK_SPEED;
	move.timeRemaining -= timeTaken;

	camComponent->camera().set_position(dest);
}

void MovementFunctions::do_navmesh_move(const Entity_Ptr& entity, Move& move, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree,
										const NavMesh_Ptr& navMesh)
{
#if 1
	// NYI
	do_direct_move(entity, move, tree);
#else
	// Step 1:		Project the movement vector onto the plane of the current nav polygon.

	int curNavPolyIndex = entity->nav_component()->cur_nav_poly_index();
	int curColPolyIndex = navMesh->polygons()[curNavPolyIndex]->collision_poly_index();
	const CollisionPolygon& curPoly = *polygons[curColPolyIndex];
	Plane plane = make_plane(curPoly);
	Vector3d dir = project_vector_onto_plane(move.dir, plane);

	// Step 2:		Check whether the new movement vector goes through the influence zone of any of the out navlinks.

	// TODO
	int hitNavlink = -1;

	// Step 3.a:	If the new movement vector doesn't hit a navlink, check whether the other end of the movement vector is within the current polygon.
	//
	//				-	If yes, move there, set the time remaining to zero and return.
	//
	//				-	If no, find the point on the boundary where we leave the polygon. Move there, and decrease
	//					the time remaining appropriately. If there's any time remaining, we are either leaving the
	//					navmesh or hitting a wall, so do a direct move in case it's the latter and return.

	if(hitNavlink == -1)
	{
		// FIXME: Walking speed will eventually be a property of the entity.
		const double WALK_SPEED = 5.0;	// in units/s

		ICameraComponent_Ptr camComponent = entity->camera_component();

		const Vector3d& source = camComponent->camera().position();
		Vector3d dest = source + move.dir * WALK_SPEED * move.timeRemaining;
		if(point_in_polygon(dest, curPoly))
		{
			camComponent->camera().set_position(dest);
			move.timeRemaining = 0;
		}
		else
		{
			// TODO
		}

		return;
	}

	// Step 3.b:	If the new movement vector hits a navlink, move to the point at which it first enters the influence zone,
	//				and reduce the time remaining appropriately. Next, calculate the time (for this entity) to traverse the
	//				navlink completely. Traverse it as much as we are able in the time remaining and decrease the time
	//				remaining appropriately. If we traversed it completely, update the current nav polygon.

	// TODO
#endif
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
