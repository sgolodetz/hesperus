/***
 * hesperus: CmpSpriteRender.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpSpriteRender.h"

#include "ICmpPosition.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpSpriteRender::CmpSpriteRender(const std::string& spriteName, double width, double height)
:	m_spriteName(spriteName), m_width(width), m_height(height)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpSpriteRender::load(const Properties& properties)
{
	// NYI
	throw 23;
}

//#################### PUBLIC METHODS ####################
void CmpSpriteRender::check_dependencies() const
{
	check_dependency<ICmpPosition>();
}

void CmpSpriteRender::render() const
{
	// NYI
	throw 23;
}

Properties CmpSpriteRender::save() const
{
	// NYI
	throw 23;
}

}
