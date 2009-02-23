/***
 * hesperus: MinimusGotoPositionYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MinimusGotoPositionYoke.h"

#include <source/level/entities/BipedWalkCommand.h>
#include <source/level/entities/MovementFunctions.h>
#include <source/level/nav/GlobalPathfinder.h>
#include <source/math/Constants.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
MinimusGotoPositionYoke::MinimusGotoPositionYoke(const Entity_Ptr& biped, const Vector3d& dest)
:	m_biped(biped), m_dest(dest)
{
	if(!m_biped->camera_component() ||
	   !m_biped->collision_component() ||
	   !m_biped->nav_component() ||
	   !m_biped->physics_component())
	{
		throw Exception("Couldn't attach a biped yoke to a non-biped");
	}
}

//#################### PUBLIC METHODS ####################
std::vector<EntityCommand_Ptr> MinimusGotoPositionYoke::generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree,
																		  const std::vector<NavDataset_Ptr>& navDatasets)
{
	// Check to make sure the yoke's still active.
	if(m_state != YOKE_ACTIVE)
	{
		return std::vector<EntityCommand_Ptr>();
	}

	const Vector3d& source = m_biped->camera_component()->camera().position();

	if(!m_path)
	{
		ICollisionComponent_Ptr colComponent = m_biped->collision_component();
		int mapIndex = colComponent->aabb_indices()[colComponent->pose()];
		NavMesh_Ptr navMesh = navDatasets[mapIndex]->nav_mesh();
		GlobalPathfinder pathfinder(navMesh, navDatasets[mapIndex]->adjacency_list(), navDatasets[mapIndex]->path_table());

		int suggestedSourcePoly = m_biped->nav_component()->cur_nav_poly_index();
		int sourcePoly = MovementFunctions::find_nav_polygon(source, suggestedSourcePoly, polygons, tree, navMesh);
		if(sourcePoly == -1)	{ m_state = YOKE_FAILED; return std::vector<EntityCommand_Ptr>(); }
		int destPoly = MovementFunctions::find_nav_polygon(m_dest, -1, polygons, tree, navMesh);
		if(destPoly == -1)		{ m_state = YOKE_FAILED; return std::vector<EntityCommand_Ptr>(); }

		// FIXME: It's wasteful to copy the array of links each time (even though it's an array of pointers).
		m_links = navMesh->links();

		m_path.reset(new std::list<int>);
		bool pathFound = pathfinder.find_path(source, sourcePoly, m_dest, destPoly, *m_path);
		if(!pathFound)			{ m_state = YOKE_FAILED; return std::vector<EntityCommand_Ptr>(); }
	}

	// Step 1:	If the path is non-empty, go to the next link that we haven't already passed.
	Vector3d dir;
	while(!m_path->empty())
	{
		const Vector3d& linkIn = m_links[m_path->front()]->source_position();
		const Vector3d& linkOut = m_links[m_path->front()]->dest_position();
		dir = linkIn - source;
		if(dir.length() >= 0.1 && source.distance(linkOut) >= 0.1)
		{
			dir.normalize();
			break;
		}
		else
		{
			m_path->pop_front();
		}
	}

	if(!m_path->empty())
	{
		std::vector<EntityCommand_Ptr> commands;
		commands.push_back(EntityCommand_Ptr(new BipedWalkCommand(m_biped, dir)));
		return commands;
	}

	// Step 2:	If the path is empty, go straight to the actual destination.
	dir = m_dest - source;
	if(dir.length() >= 0.1)
	{
		dir.normalize();

		std::vector<EntityCommand_Ptr> commands;
		commands.push_back(EntityCommand_Ptr(new BipedWalkCommand(m_biped, dir)));
		return commands;
	}
	else
	{
		// We've reached the destination.
		m_state = YOKE_SUCCEEDED;
		return std::vector<EntityCommand_Ptr>();
	}
}

}
