/***
 * hesperus: CmpSpriteRender.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpSpriteRender.h"

#include <source/level/sprites/Sprite.h>
#include <source/level/sprites/SpriteManager.h>
#include "ICmpPosition.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpSpriteRender::CmpSpriteRender(const std::string& spriteName, double width, double height)
:	m_spriteName(spriteName), m_width(width), m_height(height)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpSpriteRender::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpSpriteRender(properties.get<std::string>("SpriteName"),
													properties.get<double>("Width"),
													properties.get<double>("Height")));
}

//#################### PUBLIC METHODS ####################
void CmpSpriteRender::check_dependencies() const
{
	check_dependency<ICmpPosition>();
}

void CmpSpriteRender::render() const
{
	ICmpPosition_CPtr cmpPosition = m_objectManager->get_component(m_objectID, cmpPosition);
	Sprite_CPtr sprite = m_objectManager->sprite_manager()->sprite(m_spriteName);
	sprite->render(cmpPosition->position(), m_width, m_height);
}

Properties CmpSpriteRender::save() const
{
	Properties properties;
	properties.set("Height", m_height);
	properties.set("SpriteName", m_spriteName);
	properties.set("Width", m_width);
	return properties;
}

}
