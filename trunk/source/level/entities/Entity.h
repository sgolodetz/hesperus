/***
 * hesperus: Entity.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITY
#define H_HESP_ENTITY

#include <iosfwd>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

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
	int id() const
	{
		return m_id;
	}

	void set_id(int id)
	{
		m_id = id;
	}
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Entity> Entity_Ptr;

}

#endif
