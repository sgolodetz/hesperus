/***
 * hesperus: YokeComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_YOKECOMPONENT
#define H_HESP_YOKECOMPONENT

#include <source/io/FieldIO.h>
#include "IYokeComponent.h"

namespace hesp {

class YokeComponent : public IYokeComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	Yoke_Ptr m_yoke;
	std::string m_yokeType;

	//#################### CONSTRUCTORS ####################
public:
	YokeComponent(const Yoke_Ptr& yoke, const std::string& yokeType)
	:	m_yoke(yoke), m_yokeType(yokeType)
	{}

	//#################### PUBLIC METHODS ####################
public:
	void save(std::ostream& os) const
	{
		FieldIO::write_typed_field(os, "Yoke", m_yokeType);
	}

	void set_yoke(const Yoke_Ptr& yoke, const std::string& yokeType)
	{
		m_yoke = yoke;
		m_yokeType = yokeType;
	}

	const Yoke_Ptr& yoke() const
	{
		return m_yoke;
	}

	const std::string& yoke_type() const
	{
		return m_yokeType;
	}
};

}

#endif
