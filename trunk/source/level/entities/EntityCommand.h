/***
 * hesperus: EntityCommand.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITYCOMMAND
#define H_HESP_ENTITYCOMMAND

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

class EntityCommand
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~EntityCommand() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void execute(int milliseconds) = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<EntityCommand> EntityCommand_Ptr;

}

#endif
