/***
 * hesperus: IOUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "IOUtil.h"

namespace hesp {

//#################### READING METHODS ####################
void IOUtil::read_counted_planes(std::istream& is, std::vector<Plane>& planes)
{
	// NYI
	throw 23;
}

Plane IOUtil::read_plane(const std::string& line, const std::string& n)
{
	// NYI
	throw 23;
}

void IOUtil::read_planes(std::istream& is, std::vector<Plane>& planes)
{
	// NYI
	throw 23;
}

void IOUtil::read_uncounted_planes(std::istream& is, std::vector<Plane>& planes)
{
	// NYI
	throw 23;
}

//#################### WRITING METHODS ####################
void IOUtil::write_planes(std::ostream& os, const std::vector<Plane>& planes, bool writeCount)
{
	if(writeCount) os << planes.size() << '\n';
	for(size_t i=0, size=planes.size(); i<size; ++i)
	{
		os << planes[i] << '\n';
	}
}

}
