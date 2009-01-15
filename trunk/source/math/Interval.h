/***
 * hesperus: Interval.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MATH_INTERVAL
#define H_HESP_MATH_INTERVAL

namespace hesp {

class Interval
{
	//#################### PRIVATE VARIABLES ####################
private:
	bool m_empty;
	double m_low, m_high;

	//#################### CONSTRUCTORS ####################
public:
	Interval(double low, double high);

	//#################### PUBLIC METHODS ####################
public:
	bool empty() const;
	double high() const;
	Interval intersect(const Interval& rhs) const;
	double low() const;
	bool overlaps(const Interval& rhs) const;
};

}

#endif
