/***
 * hesperus: VisTable.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSP_VISTABLE
#define H_HESP_BSP_VISTABLE

#include <vector>

namespace hesp {

/**
This class template represents a visibility table. It stores the
visibility relation for an ordered set of objects (note that for
the purposes of the PVS calculator, it can store the visibility
relation for both portals and leaves).
*/
template <typename T>
class VisTable
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<std::vector<T> > m_table;

	//#################### CONSTRUCTORS ####################
public:
	VisTable(int n, const T& initialValue = T());

	//#################### PUBLIC OPERATORS ####################
public:
	T& operator()(int i, int j);
	const T& operator()(int i, int j) const;

	//#################### PUBLIC METHODS ####################
public:
	int size() const;
};

}

#include "VisTable.tpp"

#endif
