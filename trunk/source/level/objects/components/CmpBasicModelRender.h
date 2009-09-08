/***
 * hesperus: CmpBasicModelRender.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPBASICMODELRENDER
#define H_HESP_CMPBASICMODELRENDER

#include "CmpModelRender.h"
#include "ICmpBasicModelRender.h"
#include "ICmpUsable.h"

namespace hesp {

class CmpBasicModelRender : public ICmpBasicModelRender, public CmpModelRender
{
	//#################### CONSTRUCTORS ####################
public:
	explicit CmpBasicModelRender(const std::string& modelName);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	void check_dependencies() const;
	void render() const;
	void render_child() const;
	void render_first_person() const;
	std::pair<std::string,Properties> save() const;
	void update_animation(int milliseconds, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const std::vector<NavDataset_Ptr>& navDatasets);
	void update_child_animation(int milliseconds, const BoneHierarchy_Ptr& parent, const std::string& parentBoneName, const RBTMatrix_CPtr& parentMatrix, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const std::vector<NavDataset_Ptr>& navDatasets);

	//#################### PRIVATE METHODS ####################
private:
	static RBTMatrix_CPtr construct_first_person_matrix();
	static RBTMatrix_CPtr construct_model_matrix(const Vector3d& p, const Vector3d& n, const Vector3d& u, const Vector3d& v);
	void update_hotspots(const ICmpUsable_Ptr& cmpItemUsable, const RBTMatrix_CPtr& parentMatrix) const;
};

}

#endif
