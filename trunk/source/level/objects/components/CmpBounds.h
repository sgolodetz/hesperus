/***
 * hesperus: CmpBounds.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPBOUNDS
#define H_HESP_CMPBOUNDS

#include "ICmpBounds.h"

namespace hesp {

class CmpBounds : public ICmpBounds
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_boundsGroup;
	std::string m_posture;

	//#################### CONSTRUCTORS ####################
public:
	CmpBounds(const std::string& boundsGroup, const std::string& posture);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	const std::string& bounds_group() const;
	const std::string& posture() const;
	Properties save() const;
	void set_posture(const std::string& posture);
};

}

#endif
