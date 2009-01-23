/***
 * hesperus: StepDownLink.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_STEPDOWNLINK
#define H_HESP_STEPDOWNLINK

#include "StepLink.h"

namespace hesp {

class StepDownLink : public StepLink
{
	//#################### CONSTRUCTORS ####################
public:
	StepDownLink(int sourcePoly, int destPoly, const Vector3d& s1, const Vector3d& s2, const Vector3d& d1, const Vector3d& d2);

	//#################### PUBLIC METHODS ####################
public:
	std::string link_name() const;
	static NavLink_Ptr load(const std::string& line);
};

}

#endif
