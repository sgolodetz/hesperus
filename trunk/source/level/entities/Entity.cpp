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
:	m_entityType(properties.entityType),
	m_aabbIndices(properties.aabbIndices),
	m_characterModel(properties.characterModel),
	m_health(properties.health),
	m_mass(properties.mass),
	m_pose(properties.pose),
	m_velocity(properties.velocity)
{
	if(properties.position && properties.look)
	{
		m_camera.reset(new VariableCamera(*properties.position, *properties.look));
	}

	if(properties.characterModel)
	{
		m_characterAnimController.reset(new AnimationController);
	}

	if(properties.aabbIndices)
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

const std::string& Entity::entity_type() const
{
	return m_entityType;
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

const std::list<OnionUtil::Transition_Ptr>& Entity::recent_transitions() const
{
	return *m_recentTransitions;
}

double Entity::run_speed() const
{
	// FIXME: This should be loaded in the constructor.
	return 10.0;	// in units/s
}

void Entity::save(std::ostream& os) const
{
	os << "Instance " << m_entityType << '\n';
	os << "{\n";

	// FIXME: Do this properly (requires changing the file format).
	if(m_characterModel) FieldIO::write_typed_field(os, "GameModel", *m_characterModel);
	if(m_camera) FieldIO::write_typed_field(os, "Position", m_camera->position());
	if(m_camera) FieldIO::write_typed_field(os, "Look", m_camera->n());
	if(m_aabbIndices) FieldIO::write_intarray_field(os, "AABBs", *m_aabbIndices);
	if(m_pose) FieldIO::write_typed_field(os, "Pose", *m_pose);
	if(m_health) FieldIO::write_typed_field(os, "Health", *m_health);
	if(m_mass) FieldIO::write_typed_field(os, "Mass", *m_mass);
	if(m_yokeType) FieldIO::write_typed_field(os, "Yoke", *m_yokeType);

	os << "}\n";
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
