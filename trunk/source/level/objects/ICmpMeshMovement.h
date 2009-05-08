/***
 * hesperus: ICmpMeshMovement.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPMESHMOVEMENT
#define H_HESP_ICMPMESHMOVEMENT

#include "ICmpDirectMovement.h"

namespace hesp {

class ICmpMeshMovement : public virtual ICmpDirectMovement
{
	//#################### NESTED CLASSES ####################
public:
	struct Traversal
	{
		const int linkIndex;
		const Vector3d source;
		const double t;

		Traversal(int linkIndex_, const Vector3d& source_, double t_)
		:	linkIndex(linkIndex_), source(source_), t(t_)
		{}
	};

	//#################### TYPEDEFS ####################
public:
	typedef shared_ptr<Traversal> Traversal_Ptr;
	typedef shared_ptr<const Traversal> Traversal_CPtr;

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual int cur_nav_poly_index() const = 0;
	virtual Traversal_CPtr cur_traversal() const = 0;
	virtual void set_cur_nav_poly_index(int curNavPolyIndex) = 0;
	virtual void set_cur_traversal(const Traversal_Ptr& curTraversal) = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpMeshMovement> ICmpMeshMovement_Ptr;
typedef shared_ptr<const ICmpMeshMovement> ICmpMeshMovement_CPtr;

}

#endif
