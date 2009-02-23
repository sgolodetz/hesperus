/***
 * hesperus: INavComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_INAVCOMPONENT
#define H_HESP_INAVCOMPONENT

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/vectors/Vector3.h>

namespace hesp {

class INavComponent
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

	//#################### DESTRUCTOR ####################
public:
	virtual ~INavComponent() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual int cur_nav_poly_index() const = 0;
	virtual Traversal_Ptr cur_traversal() const = 0;
	virtual void set_cur_nav_poly_index(int curNavPolyIndex) = 0;
	virtual void set_cur_traversal(const Traversal_Ptr& traversal) = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<INavComponent> INavComponent_Ptr;

}

#endif
