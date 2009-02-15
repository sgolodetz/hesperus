/***
 * hesperus: YokeComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_YOKECOMPONENT
#define H_HESP_YOKECOMPONENT

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
	YokeComponent(const Yoke_Ptr& yoke, const std::string& yokeType);

	//#################### PUBLIC METHODS ####################
public:
	void save(std::ostream& os) const;
	void set_yoke(const Yoke_Ptr& yoke, const std::string& yokeType);
	const Yoke_Ptr& yoke() const;
	const std::string& yoke_type() const;
};

}

#endif
