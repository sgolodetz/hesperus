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
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	AnimationController_CPtr anim_controller() const;
	void check_dependencies() const;
	const std::string& model_name() const;
	void render() const;
	std::pair<std::string,Properties> save() const;
	void set_highlights(bool enabled);
	void set_model_manager(const ModelManager_Ptr& modelManager);
	void set_skeleton();
	void update_animation(int milliseconds, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets);

	//#################### PRIVATE METHODS ####################
private:
	static RBTMatrix_CPtr construct_model_matrix(const Vector3d& p, const Vector3d& n, const Vector3d& u, const Vector3d& v);
	void render_aabb(const Vector3d& p) const;
};

}

#endif
