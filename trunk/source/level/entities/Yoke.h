/***
 * hesperus: Yoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_YOKE
#define H_HESP_YOKE

#include <vector>

#include "EntityCommand.h"

namespace hesp {

class Yoke
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~Yoke() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual std::vector<EntityCommand_Ptr> generate_commands(/*const Input& input*/) = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Yoke> Yoke_Ptr;

}

#endif
