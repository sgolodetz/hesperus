/***
 * hesperus: IComponent.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "IComponent.h"

namespace hesp {

//#################### DESTRUCTOR ####################
IComponent::~IComponent() {}

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
