/***
 * hesperus: CmpBasicProjectile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpBasicProjectile.h"

#include <source/level/objects/messages/MsgObjectDestroyed.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpBasicProjectile::CmpBasicProjectile(const ObjectID& firer)
:	m_firer(firer)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpBasicProjectile::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpBasicProjectile(properties.get<ObjectID>("Firer")));
}

//#################### PUBLIC METHODS ####################
const ObjectID& CmpBasicProjectile::firer() const
{
	return m_firer;
}

void CmpBasicProjectile::process_message(const MsgObjectDestroyed& msg)
{
	if(msg.object_id() == m_firer)
	{
		m_firer = ObjectID();	// invalidate the ID we're holding: the specified object no longer exists
	}
}

void CmpBasicProjectile::register_listening()
{
	m_objectManager->add_listener(this, m_firer);
}

Properties CmpBasicProjectile::save() const
{
	Properties properties;
	properties.set("Firer", m_firer);
	return properties;
}

}
