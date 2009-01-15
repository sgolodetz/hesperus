/***
 * hesperus: Interval.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Interval.h"

#include <algorithm>

#include "Constants.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
/**
Constructs an Interval representing [low,high].

Preconditions:

- high > low + EPSILON

@param low		The low end of the interval
@param high		The high end of the interval
*/
Interval::Interval(double low, double high)
:	m_low(low), m_high(high)
{
	m_empty = m_high - m_low < EPSILON;
}

//#################### PUBLIC METHODS ####################
bool Interval::empty() const	{ return m_empty; }
double Interval::high() const	{ return m_high; }

/**
Calculates the intersection of this interval with another.

@param rhs	The other interval
@return		The intersection of the two intervals
*/
Interval Interval::intersect(const Interval& rhs) const
{
	double largerLow = std::max(m_low, rhs.m_low);
	double smallerHigh = std::min(m_high, rhs.m_high);
	return Interval(largerLow, smallerHigh);
}

double Interval::low() const	{ return m_low; }

/**
Determines whether this interval overlaps another.

@param rhs	The other interval
@return		true, if they overlap, or false otherwise
*/
bool Interval::overlaps(const Interval& rhs) const
{
	return !(m_low >= rhs.m_high || rhs.m_low >= m_high);
}

}
