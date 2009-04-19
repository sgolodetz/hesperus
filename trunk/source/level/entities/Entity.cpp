/***
 * hesperus: Entity.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Entity.h"

#include <source/camera/VariableCamera.h>
#include <source/io/util/FieldIO.h>
#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Entity::Entity(const Properties& properties)
{
	if(properties.has("Archetype")) m_archetype = *properties.get<std::string>("Archetype");
	else throw Exception("Missing archetype");

	m_aabbIndices = properties.get<std::vector<int> >("AABBs");
	m_characterModel = properties.get<std::string>("GameModel");
	m_health = properties.get<int>("Health");
	m_mass = properties.get<double>("Mass");
	m_pose = properties.get<int>("Pose");

	if(properties.has("Position") && properties.has("Look"))
	{
		m_camera.reset(new VariableCamera(*properties.get<Vector3d>("Position"), *properties.get<Vector3d>("Look")));
		m_velocity.reset(new Vector3d);
	}

	if(m_characterModel)
	{
		m_characterAnimController.reset(new AnimationController);
	}

	if(m_aabbIndices)
	{
		m_curNavPolyIndex.reset(new int(-1));
		m_recentTransitions.reset(new std::list<OnionUtil::Transition_Ptr>());
	}
}

//#################### PUBLIC METHODS ####################
const std::vector<int>& Entity::aabb_indices() const
{
	return *m_aabbIndices;
}

const std::string& Entity::archetype() const
{
	return m_archetype;
}

Camera& Entity::camera()
{
	return *m_camera;
}

const AnimationController_Ptr& Entity::character_anim_controller() const
{
	return m_characterAnimController;
}

const std::string& Entity::character_model() const
{
#if 0
	return *m_characterModel;
#else
	// FIXME: Load the appropriate models here once they've been created.
	static std::string s = "Test-15";
	return s;
#endif
}

int Entity::cur_nav_poly_index() const
{
	return *m_curNavPolyIndex;
}

Entity::Traversal_CPtr Entity::cur_traversal() const
{
	return m_curTraversal;
}

bool Entity::is_animatable() const
{
	return m_camera && m_characterAnimController && m_characterModel;
}

bool Entity::is_biped() const
{
	return m_aabbIndices && m_camera && m_characterAnimController && m_characterModel && m_curNavPolyIndex && m_health && m_mass && m_pose && m_recentTransitions && m_velocity;
}

bool Entity::is_simulable() const
{
	return m_aabbIndices && m_camera && m_mass && m_pose && m_recentTransitions && m_velocity;
}

int Entity::pose() const
{
	return *m_pose;
}

const Vector3d& Entity::position() const
{
	return m_camera->position();
}

Properties Entity::properties() const
{
	Properties ret;

	ret.set("Archetype", m_archetype);

	ret.set("AABBs", m_aabbIndices);
	ret.set("GameModel", m_characterModel);
	ret.set("Health", m_health);
	ret.set("Look", m_camera->n());
	ret.set("Mass", m_mass);
	ret.set("Pose", m_pose);
	ret.set("Position", m_camera->position());
	ret.set("Yoke", m_yokeType);

	return ret;
}

const std::list<OnionUtil::Transition_Ptr>& Entity::recent_transitions() const
{
	return *m_recentTransitions;
}

double Entity::run_speed() const
{
	// FIXME: This should be loaded in the constructor.
	return 10.0;	// in units/s
}

void Entity::set_cur_nav_poly_index(int curNavPolyIndex)
{
	m_curNavPolyIndex.reset(new int(curNavPolyIndex));
}

void Entity::set_cur_traversal(const Traversal_Ptr& curTraversal)
{
	m_curTraversal = curTraversal;
}

void Entity::set_pose(int pose)
{
	m_pose.reset(new int(pose));
}

void Entity::set_position(const Vector3d& position)
{
	m_camera->set_position(position);
}

void Entity::set_velocity(const Vector3d& velocity)
{
	m_velocity.reset(new Vector3d(velocity));
}

void Entity::set_yoke(const Yoke_Ptr& yoke, const std::string& yokeType)
{
	m_yoke = yoke;
	m_yokeType.reset(new std::string(yokeType));
}

void Entity::update_recent_transitions(const OnionUtil::Transition_Ptr& transition)
{
	assert(transition->location != NULL);

	// Remove any recent transition planes which the entity's no longer on.
	const Vector3d& location = *transition->location;
	for(std::list<OnionUtil::Transition_Ptr>::iterator it=m_recentTransitions->begin(), iend=m_recentTransitions->end(); it!=iend;)
	{
		if(classify_point_against_plane(location, *(*it)->plane) == CP_COPLANAR) ++it;
		else it = m_recentTransitions->erase(it);
	}

	// Add the latest transition.
	m_recentTransitions->push_front(transition);
}

const Vector3d& Entity::velocity() const
{
	return *m_velocity;
}

double Entity::walk_speed() const
{
	// FIXME: This should be loaded in the constructor.
	return 5.0;		// in units/s
}

const Yoke_Ptr& Entity::yoke() const
{
	return m_yoke;
}

}
