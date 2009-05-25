/***
 * hesperus: CmpModelRender.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPMODELRENDER
#define H_HESP_CMPMODELRENDER

#include "ICmpModelRender.h"

namespace hesp {

class CmpModelRender : public ICmpModelRender
{
	//#################### PRIVATE VARIABLES ####################
private:
	AnimationController_Ptr m_animController;
	bool m_highlights;
	ModelManager_Ptr m_modelManager;
	std::string m_modelName;

	//#################### CONSTRUCTORS ####################
public:
	explicit CmpModelRender(const std::string& modelName);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	AnimationController_Ptr anim_controller() const;
	void check_dependencies() const;
	void render() const;
	std::pair<std::string,Properties> save() const;
	void set_highlights(bool enabled);
	void set_model_manager(const ModelManager_Ptr& modelManager);
	void set_skeleton();
};

}

#endif
