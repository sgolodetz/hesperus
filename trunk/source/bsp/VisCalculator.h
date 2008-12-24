/***
 * hesperus: VisCalculator.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSP_VISCALCULATOR
#define H_HESP_BSP_VISCALCULATOR

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "Portal.h"
#include "VisTable.h"

namespace hesp {

class VisCalculator
{
	//#################### ENUMERATIONS ####################
public:
	enum PortalVisState
	{
		VS_NO,				// these portals definitely can't see each other
		VS_INITIALMAYBE,	// after the initial calculations, it's still possible these portals can see each other
		VS_FLOODFILLMAYBE,	// after the flood fill, it's still possible these portals can see each other
		VS_YES				// these portals definitely can see each other
	};

	//#################### TYPEDEFS ####################
private:
	typedef VisTable<PortalVisState> PortalVisTable;
	typedef shared_ptr<PortalVisTable> PortalVisTable_Ptr;
	typedef VisTable<bool> LeafVisTable;
	typedef shared_ptr<LeafVisTable> LeafVisTable_Ptr;

	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<Portal_Ptr> m_portals;
	PortalVisTable_Ptr m_portalVis;
	LeafVisTable_Ptr m_leafVis;

	//#################### CONSTRUCTORS ####################
public:
	VisCalculator(const std::vector<Portal_Ptr>& portals);

	//#################### PUBLIC METHODS ####################
public:
	LeafVisTable_Ptr calculate_leaf_vis_table();

	//#################### PRIVATE METHODS ####################
private:
	void calculate_portal_pvs(const Portal_Ptr& originalSource);
	void clean_intermediate();
	void flood_fill();
	void flood_from(const Portal_Ptr& originalSource);
	void full_portal_vis();
	void initial_portal_vis();
	void portal_to_leaf_vis();
};

}

#endif
