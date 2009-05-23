/***
 * hesperus: IComponent.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "IComponent.h"

namespace hesp {

//#################### DESTRUCTOR ####################
IComponent::~IComponent() {}

//#################### PUBLIC METHODS ####################
void IComponent::check_dependencies() const
{
	// Note:	A default implementation is provided so that components which don't have any dependencies
	//			to check aren't required to provide a stub implementation.
}

//#################### PROTECTED METHODS ####################
void IComponent::set_object_id(const ObjectID& objectID)
{
	m_objectID = objectID;
}

void IComponent::set_object_manager(ObjectManager *objectManager)
{
	m_objectManager = objectManager;
}

}
