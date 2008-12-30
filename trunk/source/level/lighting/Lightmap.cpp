/***
 * hesperus: Lightmap.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Lightmap.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Lightmap::Lightmap(int rows, int cols)
:	m_lumels(rows)
{
	for(int r=0; r<rows; ++r)
	{
		m_lumels[r].resize(cols);
	}
}

//#################### PUBLIC OPERATORS ####################
Colour3d& Lightmap::operator()(int row, int col)
{
	return m_lumels[row][col];
}

const Colour3d& Lightmap::operator()(int row, int col) const
{
	return m_lumels[row][col];
}

Lightmap& Lightmap::operator+=(const Lightmap& rhs)
{
	// NYI
	throw 23;
}

}
