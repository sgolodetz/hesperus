/***
 * hesperus: Yoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_YOKE
#define H_HESP_YOKE

#include <vector>

#include <source/level/nav/NavDataset.h>
#include <source/util/UserInput.h>
#include "EntityCommand.h"

namespace hesp {

class Yoke
{
	//#################### ENUMERATIONS ####################
public:
	enum State
	{
		YOKE_ACTIVE,		// the yoke's still in the process of generating commands
		YOKE_FAILED,		// the yoke is no longer generating commands because it failed
		YOKE_SUCCEEDED		// the yoke is no longer generating commands because it succeeded
	};

	//#################### PROTECTED VARIABLES ####################
protected:
	State m_state;

	//#################### CONSTRUCTORS ####################
public:
	Yoke() : m_state(YOKE_ACTIVE) {}

	//#################### DESTRUCTOR ####################
public:
	virtual ~Yoke() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual std::vector<EntityCommand_Ptr> generate_commands(UserInput& input, const std::vector<NavDataset_Ptr>& navDatasets) = 0;

	//#################### PUBLIC METHODS ####################
public:
	State state() const
	{
		return m_state;
	}
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Yoke> Yoke_Ptr;

}

#endif
