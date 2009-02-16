/***
 * hesperus: MinimusGotoPositionYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MinimusGotoPositionYoke.h"

#include <source/level/entities/BipedWalkCommand.h>
#include <source/level/nav/GlobalPathfinder.h>
#include <source/math/Constants.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
MinimusGotoPositionYoke::MinimusGotoPositionYoke(const Entity_Ptr& biped)
:	m_biped(biped)
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
std::vector<EntityCommand_Ptr> MinimusGotoPositionYoke::generate_commands(UserInput& input, const std::vector<NavDataset_Ptr>& navDatasets)
{
	// FIXME: These should be passed into the constructor somehow.
	static Vector3d source(20,20,6);
	static int sourcePoly = 5;
	static Vector3d dest(14,6,6);
	static int destPoly = 0;

	if(!m_path)
	{
		ICollisionComponent_Ptr colComponent = m_biped->collision_component();
		int mapIndex = colComponent->aabb_indices()[colComponent->pose()];
		GlobalPathfinder pathfinder(navDatasets[mapIndex]->nav_mesh(), navDatasets[mapIndex]->adjacency_list(), navDatasets[mapIndex]->path_table());

		// FIXME: It's wasteful to copy the array of links each time (even though it's an array of pointers).
		m_links = navDatasets[mapIndex]->nav_mesh()->links();

		// FIXME: Do something useful if a path can't be found here.
		m_path.reset(new std::list<int>);
		pathfinder.find_path(source, sourcePoly, dest, destPoly, *m_path);
	}

	const Vector3d& position = m_biped->camera_component()->camera().position();

	// Step 1:	If the path is non-empty, go to the next link that we haven't already passed.
	Vector3d dir;
	while(!m_path->empty())
	{
		const Vector3d& linkIn = m_links[m_path->front()]->source_position();
		const Vector3d& linkOut = m_links[m_path->front()]->dest_position();
		dir = linkIn - position;
		if(dir.length() >= 0.1 && position.distance(linkOut) >= 0.1)
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
	dir = dest - position;
	if(dir.length() >= 0.1)
	{
		dir.normalize();

		std::vector<EntityCommand_Ptr> commands;
		commands.push_back(EntityCommand_Ptr(new BipedWalkCommand(m_biped, dir)));
		return commands;
	}
	else
	{
		// FIXME: Do something useful when we reach the destination.
		return std::vector<EntityCommand_Ptr>();
	}
}

}
