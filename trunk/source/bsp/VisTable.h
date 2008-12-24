/***
 * hesperus: VisTable.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSP_VISTABLE
#define H_HESP_BSP_VISTABLE

namespace hesp {

/**
This class template represents a visibility table. It stores the
visibility relation for an ordered set of objects (note that for
the purposes of the PVS calculator, it can store the visibility
relation for both portals and leaves). The client code can choose
between an uncompressed or compressed visibility table at compile
time.
*/
template <bool Compressed> class VisTable;

//#################### UNCOMPRESSED VISTABLE ####################
template <> class VisTable<false>
{
	//#################### CONSTRUCTORS ####################
public:
	// TODO

	//#################### PUBLIC METHODS ####################
public:
	// TODO
};

//#################### COMPRESSED VISTABLE ####################
template <> class VisTable<true>
{
	//#################### CONSTRUCTORS ####################
public:
	// TODO

	//#################### PUBLIC METHODS ####################
public:
	// TODO
};

}

#endif
