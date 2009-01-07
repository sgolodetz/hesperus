/***
 * hesperus: OnionCompiler.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_ONIONBSP_ONIONCOMPILER
#define H_HESP_LEVEL_ONIONBSP_ONIONCOMPILER

#include <map>
#include <set>
#include <vector>

#include <source/math/geom/Plane.h>
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
	typedef std::set<OnionPlane_Ptr> OnionPlaneSet;

	//#################### NESTED CLASSES ####################
private:
	struct PolyIndex
	{
		int index;
		bool splitCandidate;	// is the onion plane of the referenced polygon a split candidate?

		PolyIndex(int index_, bool splitCandidate_)
		:	index(index_), splitCandidate(splitCandidate_)
		{}
	};

	//#################### PRIVATE VARIABLES ####################
private:
	// Input data
	double m_weight;

	// Intermediate data
	OnionPlaneSet m_onionPlanes;
	std::map<int,int> m_polyToOnionPlane;

	// Intermediate / output data
	PolyVector_Ptr m_polygons;

	// Output data
	OnionTree_Ptr m_tree;

	//#################### CONSTRUCTORS ####################
public:
	OnionCompiler(const std::vector<PolyVector_Ptr>& maps, double weight);

	//#################### PUBLIC METHODS ####################
public:
	void build_tree();
	const PolyVector_Ptr& polygons() const;
	const OnionTree_Ptr& tree() const;

	//#################### PRIVATE METHODS ####################
private:
	OnionNode_Ptr build_subtree(const std::vector<PolyIndex>& polyIndices, std::vector<OnionNode_Ptr>& nodes,
								const std::vector<PlaneClassifier>& relativeToClosestAncestor);
	OnionPlane_Ptr choose_split_plane(const std::vector<PolyIndex>& polyIndices);
	void determine_onion_planes(const std::vector<PolyVector_Ptr>& maps);
};

}

#include "OnionCompiler.tpp"

#endif
