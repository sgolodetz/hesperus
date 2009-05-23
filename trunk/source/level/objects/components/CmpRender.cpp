/***
 * hesperus: CmpRender.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpRender.h"

#include <source/level/objects/base/ObjectManager.h>
#include "ICmpOrientation.h"
#include "ICmpPosition.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpRender::CmpRender(const std::string& modelName)
:	m_modelName(modelName), m_animController(new AnimationController)
{}

//#################### STATIC FACTORY METHODS ####################
IComponent_Ptr CmpRender::load(const Properties& properties)
{
	return IComponent_Ptr(new CmpRender(properties.get_actual<std::string>("ModelName")));
}

//#################### PUBLIC METHODS ####################
AnimationController_Ptr CmpRender::anim_controller() const
{
	return m_animController;
}

void CmpRender::check_dependencies() const
{
	check_dependency<ICmpOrientation>();
	check_dependency<ICmpPosition>();
}

const std::string& CmpRender::model_name() const
{
	return m_modelName;
}

std::pair<std::string,Properties> CmpRender::save() const
{
	Properties properties;
	properties.set_actual("ModelName", m_modelName);
	return std::make_pair("Render", properties);
}

}
