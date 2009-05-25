/***
 * hesperus: ICmpActivatable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ICmpActivatable.h"

#include <source/level/objects/base/ObjectManager.h>
#include "ICmpAABBBounds.h"
#include "ICmpModelRender.h"
#include "ICmpPosition.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
void ICmpActivatable::check_dependencies() const
{
	check_dependency<ICmpAABBBounds>();		// need bounds against which to do picking
	check_dependency<ICmpModelRender>();
	check_dependency<ICmpPosition>();
}

}
