/***
 * hesperus: Portal.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Portal.h"

namespace hesp {

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const LeafLink& rhs)
{
	os << "[ " << rhs.fromLeaf << ' ' << rhs.toLeaf << " ]";
	return os;
}

std::istream& operator>>(std::istream& is, LeafLink& rhs)
{
	is.get();
	is.get();
	is >> rhs.fromLeaf;
	is.get();
	is >> rhs.toLeaf;
	is.get();
	is.get();
	return is;
}

}
