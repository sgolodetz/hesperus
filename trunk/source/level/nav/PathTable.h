/***
 * hesperus: PathTable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_NAV_PATHTABLE
#define H_HESP_LEVEL_NAV_PATHTABLE

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

class PathTable
{
	//#################### NESTED CLASSES ####################
private:
	struct Entry
	{
		float cost;
		int nextNode;

		Entry() : cost((float)INT_MAX), nextNode(-1) {}
	};

	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<std::vector<Entry> > m_table;
	int m_size;

	//#################### CONSTRUCTORS ####################
public:
	PathTable(int size);

	//#################### PUBLIC METHODS ####################
public:
	float& cost(int i, int j);
	float cost(int i, int j) const;
	int& next_node(int i, int j);
	int next_node(int i, int j) const;
	int size() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<PathTable> PathTable_Ptr;
typedef shared_ptr<const PathTable> PathTable_CPtr;

}

#endif
