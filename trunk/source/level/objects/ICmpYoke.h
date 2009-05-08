/***
 * hesperus: ICmpYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPYOKE
#define H_HESP_ICMPYOKE

#include <vector>

#include <source/level/nav/NavDataset.h>
#include <source/level/trees/OnionTree.h>
#include <source/util/UserInput.h>
#include "IComponent.h"
#include "ObjectCommand.h"

namespace hesp {

class ICmpYoke : public IComponent
{
	//#################### ENUMERATIONS ####################
public:
	enum State
	{
		YOKE_ACTIVE,		// the yoke is still in the process of generating commands
		YOKE_FAILED,		// the yoke is no longer generating commands because it failed
		YOKE_SUCCEEDED		// the yoke is no longer generating commands because it succeeded
	};

	//#################### PROTECTED VARIABLES ####################
protected:
	State m_state;

	//#################### CONSTRUCTORS ####################
public:
	ICmpYoke() : m_state(YOKE_ACTIVE) {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual std::vector<ObjectCommand_Ptr> generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets) = 0;

	//#################### PUBLIC METHODS ####################
public:
	State state() const
	{
		return m_state;
	}
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpYoke> ICmpYoke_Ptr;
typedef shared_ptr<const ICmpYoke> ICmpYoke_CPtr;

}

#endif
