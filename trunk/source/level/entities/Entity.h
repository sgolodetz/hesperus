/***
 * hesperus: Entity.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITY
#define H_HESP_ENTITY

#include <list>
#include <string>
#include <vector>

#include <boost/filesystem/operations.hpp>

#include <source/camera/Camera.h>
#include <source/level/models/AnimationController.h>
#include <source/level/trees/OnionUtil.h>
#include <source/math/vectors/Vector3.h>
#include <source/util/Properties.h>
#include "Yoke.h"

namespace hesp {

//#################### TYPEDEFS ####################
typedef shared_ptr<class Entity> Entity_Ptr;
typedef shared_ptr<const class Entity> Entity_CPtr;

class Entity
{
	//#################### NESTED CLASSES ####################
public:
	struct Traversal
	{
		const int linkIndex;
		const Vector3d source;
		const double t;

		Traversal(int linkIndex_, const Vector3d& source_, double t_)
		:	linkIndex(linkIndex_), source(source_), t(t_)
		{}
	};

	//#################### TYPEDEFS ####################
public:
	typedef shared_ptr<Traversal> Traversal_Ptr;
	typedef shared_ptr<const Traversal> Traversal_CPtr;

	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_archetype;

	shared_ptr<std::vector<int> >						m_aabbIndices;
	shared_ptr<Camera>									m_camera;
	shared_ptr<AnimationController>						m_characterAnimController;
	shared_ptr<std::string>								m_characterModel;

	// The index of the nav polygon in which the centre of the entity's AABB currently resides (-1 if unknown)
	shared_ptr<int>										m_curNavPolyIndex;

	// The details of the current nav link traversal (NULL if not currently traversing a link)
	shared_ptr<Traversal>								m_curTraversal;

	shared_ptr<int>										m_health;
	shared_ptr<double>									m_mass;
	shared_ptr<int>										m_pose;

	// Records the details of recent times the entity would have crossed a wall into solid space (had we not stopped it)
	shared_ptr<std::list<OnionUtil::Transition_Ptr> >	m_recentTransitions;

	shared_ptr<Vector3d>								m_velocity;
	shared_ptr<Yoke>									m_yoke;
	shared_ptr<std::string>								m_yokeType;

	//#################### CONSTRUCTORS ####################
public:
	Entity(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	const std::vector<int>& aabb_indices() const;
	const std::string& archetype() const;
	Camera& camera();
	const AnimationController_Ptr& character_anim_controller() const;
	const std::string& character_model() const;
	int cur_nav_poly_index() const;
	Traversal_CPtr cur_traversal() const;
	bool is_animatable() const;
	bool is_biped() const;
	bool is_simulable() const;
	Properties properties() const;
	int pose() const;
	const Vector3d& position() const;
	const std::list<OnionUtil::Transition_Ptr>& recent_transitions() const;
	double run_speed() const;
	void set_cur_nav_poly_index(int curNavPolyIndex);
	void set_cur_traversal(const Traversal_Ptr& curTraversal);
	void set_pose(int pose);
	void set_position(const Vector3d& position);
	void set_velocity(const Vector3d& velocity);
	void set_yoke(const Yoke_Ptr& yoke, const std::string& yokeType);
	void update_recent_transitions(const OnionUtil::Transition_Ptr& transition);
	const Vector3d& velocity() const;
	double walk_speed() const;
	const Yoke_Ptr& yoke() const;
};

}

#endif
