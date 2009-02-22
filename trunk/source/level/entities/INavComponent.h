/***
 * hesperus: INavComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_INAVCOMPONENT
#define H_HESP_INAVCOMPONENT

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

class INavComponent
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~INavComponent() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual int cur_link_index() const = 0;
	virtual int cur_nav_poly_index() const = 0;
	virtual void set_cur_link_index(int curLinkIndex) = 0;
	virtual void set_cur_nav_poly_index(int curNavPolyIndex) = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<INavComponent> INavComponent_Ptr;

}

#endif
