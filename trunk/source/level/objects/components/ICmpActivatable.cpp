/***
 * hesperus: ICmpActivatable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ICmpActivatable.h"

#include "ICmpBounds.h"
#include "ICmpModelRender.h"
#include "ICmpPosition.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
void ICmpActivatable::check_dependencies() const
{
	check_dependency<ICmpBounds>();			// need bounds against which to do picking
	check_dependency<ICmpModelRender>();
	check_dependency<ICmpPosition>();
}

}
