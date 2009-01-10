/***
 * hesperus: OnionPortal.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "OnionPortal.h"

#include <ostream>

namespace hesp {

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const OnionPortalInfo& rhs)
{
	os << "[ ";
	os << rhs.fromLeaf << ' ' << rhs.toLeaf << ' ';

	// Output the indices of the maps in which the portal appears.
	os << "{ ";
	for(std::set<int>::const_iterator it=rhs.mapIndices.begin(), iend=rhs.mapIndices.end(); it!=iend; ++it)
	{
		os << *it << ' ';
	}
	os << "}";

	os << " ]";
	return os;
}

}
