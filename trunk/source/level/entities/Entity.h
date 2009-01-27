/***
 * hesperus: Entity.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifdef _MSC_VER
	#pragma warning(disable:4250)	// disable the 'inherited by dominance' warning in VC++ (it's pointlessly warning us about doing something intentional)
#endif

#ifndef H_HESP_ENTITY
#define H_HESP_ENTITY

#include <iosfwd>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class BipedEntity;
class CollidableEntity;
class Guard;
class LocatableEntity;
class MortalEntity;
class OrientedEntity;
class Player;
class VisibleEntity;
class YokeableEntity;

class Entity
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_id;

	//#################### CONSTRUCTORS ####################
public:
	Entity() : m_id(-1) {}

	//#################### DESTRUCTOR ####################
public:
	virtual ~Entity() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void save(std::ostream& os) const = 0;

	//#################### PUBLIC METHODS ####################
public:
	virtual BipedEntity *as_biped()				{ return NULL; }
	virtual CollidableEntity *as_collidable()	{ return NULL; }
	virtual Guard *as_guard()					{ return NULL; }
	virtual LocatableEntity *as_locatable()		{ return NULL; }
	virtual MortalEntity *as_mortal()			{ return NULL; }
	virtual OrientedEntity *as_oriented()		{ return NULL; }
	virtual Player *as_player()					{ return NULL; }
	virtual VisibleEntity *as_visible()			{ return NULL; }
	virtual YokeableEntity *as_yokeable()		{ return NULL; }

	int id() const								{ return m_id; }
	void set_id(int id)							{ m_id = id; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Entity> Entity_Ptr;

}

#endif
