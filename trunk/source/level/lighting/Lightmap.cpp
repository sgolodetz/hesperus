/***
 * hesperus: Lightmap.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Lightmap.h"

#include <source/exceptions/InvalidParameterException.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Lightmap::Lightmap(int rows, int cols)
:	m_lumels(rows), m_rows(rows), m_cols(cols)
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
	if(m_rows != rhs.m_rows || m_cols != rhs.m_cols)
		throw InvalidParameterException("Lightmaps are not of the same dimensions");

	for(int r=0; r<m_rows; ++r)
	{
		for(int c=0; c<m_cols; ++c)
		{
			m_lumels[r][c] += rhs.m_lumels[r][c];
		}
	}

	return *this;
}

}
