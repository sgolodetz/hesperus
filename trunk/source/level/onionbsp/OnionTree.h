/***
 * hesperus: OnionTree.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ONIONTREE
#define H_HESP_ONIONTREE

#include <source/math/vectors/Vector3.h>
#include "OnionBranch.h"
#include "OnionLeaf.h"

namespace hesp {

//#################### TYPEDEFS ####################
typedef shared_ptr<class OnionTree> OnionTree_Ptr;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;

class OnionTree
{
	//#################### ENUMERATIONS ####################
public:
	enum RayClassifier
	{
		RAY_EMPTY,				// the ray only lies in leaves of the tree which are empty in the relevant map
		RAY_SOLID,				// the ray only lies in leaves of the tree which are solid in the relevant map
		RAY_TRANSITION_ES,		// the first transition of the ray in the relevant map is from an empty leaf to a solid one
		RAY_TRANSITION_SE		// the first transition of the ray in the relevant map is from a solid leaf to an empty one
	};

	//#################### NESTED CLASSES ####################
public:
	struct Transition
	{
		RayClassifier classifier;
		Vector3d_Ptr location;
		Plane_CPtr plane;

		explicit Transition(RayClassifier classifier_, const Vector3d_Ptr& location_ = Vector3d_Ptr(), const Plane_CPtr& plane_ = Plane_CPtr())
		:	classifier(classifier_), location(location_), plane(plane_)
		{}
	};

	//#################### TYPEDEFS ####################
public:
	typedef OnionBranch Branch;
	typedef OnionLeaf Leaf;
	typedef OnionNode Node;

	typedef shared_ptr<Transition> Transition_Ptr;

	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<OnionNode_Ptr> m_nodes;
	std::vector<OnionLeaf*> m_leaves;
	int m_mapCount;

	//#################### CONSTRUCTORS ####################
public:
	OnionTree(const std::vector<OnionNode_Ptr>& nodes, int mapCount);

	//#################### PUBLIC METHODS ####################
public:
	Transition find_first_transition(int mapIndex, const Vector3d& source, const Vector3d& dest) const;
	int find_leaf_index(const Vector3d& p) const;
	const OnionLeaf *leaf(int n) const;
	static OnionTree_Ptr load_postorder_text(std::istream& is);
	int map_count() const;
	void output_postorder_text(std::ostream& os) const;
	OnionNode_Ptr root() const;

	//#################### PRIVATE METHODS ####################
private:
	Transition find_first_transition_sub(int mapIndex, const Vector3d& source, const Vector3d& dest, const OnionNode_Ptr& node) const;
	void index_leaves();
	void index_leaves_sub(const OnionNode_Ptr& node);
};

}

#endif
