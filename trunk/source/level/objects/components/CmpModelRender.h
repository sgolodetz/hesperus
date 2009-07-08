/***
 * hesperus: CmpModelRender.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPMODELRENDER
#define H_HESP_CMPMODELRENDER

#include "ICmpModelRender.h"
#include "ICmpUsable.h"

namespace hesp {

class CmpModelRender : public ICmpModelRender
{
	//#################### NESTED CLASSES ####################
private:
	struct ProcessResults
	{
		Model_Ptr itemModel;
		RBTMatrix_CPtr mat;
		Model_Ptr model;
		Vector3d p, n, u, v;

		ProcessResults(const Model_Ptr& itemModel_, const RBTMatrix_CPtr& mat_, const Model_Ptr& model_,
					   const Vector3d& p_, const Vector3d& n_, const Vector3d& u_, const Vector3d& v_)
		:	itemModel(itemModel_), mat(mat_), model(model_), p(p_), n(n_), u(u_), v(v_)
		{}
	};

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
	AnimationController_Ptr anim_controller() const;
	void check_dependencies() const;
	const std::string& model_name() const;
	void render() const;
	void render_first_person() const;
	std::pair<std::string,Properties> save() const;
	void set_highlights(bool enabled);
	void set_model_manager(const ModelManager_Ptr& modelManager);
	void set_skeleton();

	//#################### PRIVATE METHODS ####################
private:
	static RBTMatrix_CPtr construct_item_matrix();
	static RBTMatrix_CPtr construct_model_matrix(const Vector3d& p, const Vector3d& n, const Vector3d& u, const Vector3d& v);
	ProcessResults process() const;
	Model_Ptr process_active_item(const ObjectID& activeItem, const Model_Ptr& characterModel, const RBTMatrix_CPtr& characterMatrix) const;
	void render_aabb(const Vector3d& p) const;
	static void render_crosshair();
	void render_nuv_axes(const Vector3d& p, const Vector3d& n, const Vector3d& u, const Vector3d& v) const;
	void update_active_item_hotspots(const ICmpUsable_Ptr& cmpItemUsable, const Skeleton_CPtr& itemSkeleton, const RBTMatrix_CPtr& characterMatrix) const;
};

}

#endif
