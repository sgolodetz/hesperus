/***
 * hesperus: OnionCompiler.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_ONIONBSP_ONIONCOMPILER
#define H_HESP_LEVEL_ONIONBSP_ONIONCOMPILER

#include <map>
#include <vector>

#include <source/math/geom/Plane.h>
#include <source/math/geom/UniquePlanePred.h>
#include "OnionPlane.h"
#include "OnionTree.h"

namespace hesp {

template <typename Poly>
class OnionCompiler
{
	//#################### TYPEDEFS ####################
private:
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;
	typedef shared_ptr<PolyVector> PolyVector_Ptr;

	//#################### ENUMERATIONS ####################
private:
	enum UsedFlag
	{
		UF_UNUSED,				// the plane of this polygon has not yet been used as a split plane
		UF_USED_DIFFERENTMAP,	// the plane of this polygon has been used, but the polygon it came from was in a different map
		UF_USED_SAMEMAP			// the plane of this polygon has been used, and its source polygon was in this map
	};

	//#################### NESTED CLASSES ####################
private:
	struct PolyIndex
	{
		int index;
		int mapIndex;			// which map the polygon came from
		UsedFlag usedFlag;

		PolyIndex(int index_, int mapIndex_, UsedFlag usedFlag_)
		:	index(index_), mapIndex(mapIndex_), usedFlag(usedFlag_)
		{}
	};

	//#################### PRIVATE VARIABLES ####################
private:
	// Input data
	double m_weight;
	int m_mapCount;

	// Intermediate data
	PolyVector_Ptr m_polygons;
	std::vector<PolyIndex> m_polyIndices;

	// Output data
	OnionTree_Ptr m_tree;

	//#################### CONSTRUCTORS ####################
public:
	OnionCompiler(const std::vector<PolyVector>& maps, double weight);

	//#################### PUBLIC METHODS ####################
public:
	void build_tree();
	const PolyVector_Ptr& polygons() const;
	const OnionTree_Ptr& tree() const;

	//#################### PRIVATE METHODS ####################
private:
	OnionNode_Ptr build_subtree(const std::vector<PolyIndex>& polyIndices, std::vector<OnionNode_Ptr>& nodes,
								const boost::dynamic_bitset<>& solidityDescriptor);
	OnionPlane_Ptr choose_split_plane(const std::vector<PolyIndex>& polyIndices);
};

}

#include "OnionCompiler.tpp"

#endif
