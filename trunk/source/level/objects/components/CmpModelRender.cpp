/***
 * hesperus: CmpModelRender.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpModelRender.h"

#include "ICmpAABBBounds.h"
#include "ICmpAnimChooser.h"
#include "ICmpInventory.h"
#include "ICmpOrientation.h"
#include "ICmpOwnable.h"
#include "ICmpPosition.h"
#include "ICmpUsable.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpModelRender::CmpModelRender(const std::string& modelName)
:	m_modelName(modelName), m_animController(new AnimationController), m_highlights(false)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpModelRender::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpModelRender(properties.get<std::string>("ModelName")));
}

//#################### PUBLIC METHODS ####################
AnimationController_CPtr CmpModelRender::anim_controller() const
{
	return m_animController;
}

void CmpModelRender::check_dependencies() const
{
	check_dependency<ICmpOrientation>();
	check_dependency<ICmpPosition>();
}

const std::string& CmpModelRender::model_name() const
{
	return m_modelName;
}

void CmpModelRender::render() const
{
	ProcessResults pr = process();

	// Render the models.
	glPushMatrix();
		glMultMatrixd(&pr.mat->rep()[0]);
		pr.model->render();
		if(pr.itemModel) pr.itemModel->render();
	glPopMatrix();

	if(m_highlights)
	{
		// If the object should be highlighted, render the object's AABB.
		render_aabb(pr.p);
	}

	// Render the object's NUV axes.
	render_nuv_axes(pr.p, pr.n, pr.u, pr.v);
}

void CmpModelRender::render_first_person() const
{
	ProcessResults pr = process();

	if(pr.itemModel)
	{
		// Apply the active item's *unattached* pose to the item model's skeleton for rendering.
		// (We had to apply the pose whilst it was attached to the parent model in order to get
		// the correct hotspot positions, but now we need its unattached pose instead.)
		ICmpInventory_Ptr cmpInventory = m_objectManager->get_component(m_objectID, cmpInventory);
		ObjectID activeItem = cmpInventory->active_item();
		ICmpModelRender_Ptr cmpItemRender = m_objectManager->get_component(activeItem, cmpItemRender);
		pr.itemModel->apply_pose_to_skeleton(cmpItemRender->anim_controller());

		// Render the active item.
		glPushMatrix();
		glPushAttrib(GL_ENABLE_BIT);
		glDisable(GL_DEPTH_TEST);		// disable z-buffer testing
		glDisable(GL_DEPTH_WRITEMASK);	// disable z-buffer writing
			RBTMatrix_CPtr itemMatrix = construct_item_matrix();
			glLoadMatrixd(&itemMatrix->rep()[0]);
			pr.itemModel->render();
		glPopAttrib();
		glPopMatrix();

		// Render the crosshair.
		render_crosshair();
	}
}

std::pair<std::string,Properties> CmpModelRender::save() const
{
	Properties properties;
	properties.set("ModelName", m_modelName);
	return std::make_pair("ModelRender", properties);
}

void CmpModelRender::set_highlights(bool enabled)
{
	m_highlights = enabled;
}

void CmpModelRender::set_model_manager(const ModelManager_Ptr& modelManager)
{
	m_modelManager = modelManager;
	m_modelManager->register_model(m_modelName);
}

void CmpModelRender::set_skeleton()
{
	Skeleton_Ptr skeleton = m_modelManager->model(m_modelName)->skeleton();
	m_animController->set_skeleton(skeleton);
}

void CmpModelRender::update_animation(int milliseconds, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets)
{
	ICmpAnimChooser_Ptr cmpAnimChooser = m_objectManager->get_component(m_objectID, cmpAnimChooser);
	if(cmpAnimChooser) m_animController->request_animation(cmpAnimChooser->choose_animation(polygons, tree, navDatasets));

	m_animController->update(milliseconds);
}

//#################### PRIVATE METHODS ####################
RBTMatrix_CPtr CmpModelRender::construct_item_matrix()
{
	RBTMatrix_Ptr mat = RBTMatrix::zeros();
	RBTMatrix& m = *mat;

	const int N_OFFSET = 3;		// the offset in the camera's n direction at which the item model will be rendered

	// Note:	This matrix maps x -> x, -y -> y, z -> -z, and translates by (1,-1,-N_OFFSET).
	//			Since item models are built in Blender with the direction they're supposed to
	//			point (e.g. their muzzle direction) in the z direction, and their up direction
	//			in the -y direction, and since by default we're looking down the -z axis in
	//			OpenGL with y being the up direction, this is an appropriate transformation to
	//			get the item model to render with the correct position and orientation.
	m(0,0) = 1;			/*m(0,1) = 0;*/		/*m(0,2) = 0;*/		m(0,3) = 1;
	/*m(1,0) = 0;*/		m(1,1) = -1;		/*m(1,2) = 0;*/		m(1,3) = -1;
	/*m(2,0) = 0;*/		/*m(2,1) = 0;*/		m(2,2) = -1;		m(2,3) = -N_OFFSET;

	return mat;
}

RBTMatrix_CPtr CmpModelRender::construct_model_matrix(const Vector3d& p, const Vector3d& n, const Vector3d& u, const Vector3d& v)
{
	// Project u and n into the horizontal plane (i.e. z = 0) to form uH and nH respectively.
	// Note that the camera is prevented from ever pointing directly upwards/downwards, so
	// renormalizing the resulting vectors isn't a problem.
	Vector3d uH = u;	uH.z = 0;	uH.normalize();
	Vector3d nH = n;	nH.z = 0;	nH.normalize();

	// Note:	This matrix maps x -> uH, -y -> nH, z -> z, and translates by p. Since models are
	//			built in Blender facing in the -y direction, this turns out to be exactly the
	//			transformation required to render the models with the correct position and
	//			orientation.
	RBTMatrix_Ptr mat = RBTMatrix::zeros();
	RBTMatrix& m = *mat;
	m(0,0) = uH.x;		m(0,1) = -nH.x;		/*m(0,2) = 0;*/		m(0,3) = p.x;
	m(1,0) = uH.y;		m(1,1) = -nH.y;		/*m(1,2) = 0;*/		m(1,3) = p.y;
	/*m(2,0) = 0;*/		/*m(2,1) = 0;*/		m(2,2) = 1;			m(2,3) = p.z;

	return mat;
}

CmpModelRender::ProcessResults CmpModelRender::process() const
{
	ICmpOrientation_Ptr cmpOrientation = m_objectManager->get_component(m_objectID, cmpOrientation);
	ICmpPosition_Ptr cmpPosition = m_objectManager->get_component(m_objectID, cmpPosition);

	const Vector3d& p = cmpPosition->position();
	const Vector3d& n = cmpOrientation->nuv_axes()->n();
	const Vector3d& u = cmpOrientation->nuv_axes()->u();
	const Vector3d& v = cmpOrientation->nuv_axes()->v();

	RBTMatrix_CPtr mat = construct_model_matrix(p, n, u, v);

	Model_Ptr model = m_modelManager->model(m_modelName);
	model->apply_pose_to_skeleton(m_animController);

	// Process the active item (if any), e.g. the weapon being carried.
	Model_Ptr itemModel;
	ICmpInventory_Ptr cmpInventory = m_objectManager->get_component(m_objectID, cmpInventory);
	if(cmpInventory)
	{
		ObjectID activeItem = cmpInventory->active_item();
		if(activeItem.valid())
		{
			itemModel = process_active_item(activeItem, model, mat);
		}
	}

	return ProcessResults(itemModel, mat, model, p, n, u, v);
}

Model_Ptr CmpModelRender::process_active_item(const ObjectID& activeItem, const Model_Ptr& characterModel, const RBTMatrix_CPtr& characterMatrix) const
{
	ICmpModelRender_Ptr cmpItemRender = m_objectManager->get_component(activeItem, cmpItemRender);	if(!cmpItemRender) return Model_Ptr();
	ICmpOwnable_Ptr cmpItemOwnable = m_objectManager->get_component(activeItem, cmpItemOwnable);	assert(cmpItemOwnable);

	Model_Ptr itemModel = m_modelManager->model(cmpItemRender->model_name());
	itemModel->attach_to_parent(characterModel, cmpItemOwnable->attach_point());
	itemModel->apply_pose_to_skeleton(cmpItemRender->anim_controller());
	itemModel->detach_from_parent();

	// If the item's a usable one (e.g. a weapon), update the positions and orientations of its hotspots.
	// Note that this must happen here (i.e. during rendering) because that's the only time the positions
	// and orientations of the bones are actually calculated.
	ICmpUsable_Ptr cmpItemUsable = m_objectManager->get_component(activeItem, cmpItemUsable);
	if(cmpItemUsable)
	{
		update_active_item_hotspots(cmpItemUsable, itemModel->skeleton(), characterMatrix);
	}

	return itemModel;
}


void CmpModelRender::render_aabb(const Vector3d& p) const
{
	ICmpAABBBounds_Ptr cmpBounds = m_objectManager->get_component(m_objectID, cmpBounds);
	if(!cmpBounds) return;

	const AABB3d& aabb = m_objectManager->aabbs()[cmpBounds->cur_aabb_index()];
	AABB3d tAABB = aabb.translate(p);
	const Vector3d& mins = tAABB.minimum();
	const Vector3d& maxs = tAABB.maximum();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glColor3d(1,1,0);

	glBegin(GL_QUADS);
		// Front
		glVertex3d(mins.x, mins.y, mins.z);
		glVertex3d(maxs.x, mins.y, mins.z);
		glVertex3d(maxs.x, mins.y, maxs.z);
		glVertex3d(mins.x, mins.y, maxs.z);

		// Right
		glVertex3d(maxs.x, mins.y, mins.z);
		glVertex3d(maxs.x, maxs.y, mins.z);
		glVertex3d(maxs.x, maxs.y, maxs.z);
		glVertex3d(maxs.x, mins.y, maxs.z);

		// Back
		glVertex3d(maxs.x, maxs.y, mins.z);
		glVertex3d(mins.x, maxs.y, mins.z);
		glVertex3d(mins.x, maxs.y, maxs.z);
		glVertex3d(maxs.x, maxs.y, maxs.z);

		// Left
		glVertex3d(mins.x, maxs.y, mins.z);
		glVertex3d(mins.x, mins.y, mins.z);
		glVertex3d(mins.x, mins.y, maxs.z);
		glVertex3d(mins.x, maxs.y, maxs.z);
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}

void CmpModelRender::render_crosshair()
{
	glPushMatrix();
	glPushAttrib(GL_ENABLE_BIT);

	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);		// disable z-buffer testing
	glDisable(GL_DEPTH_WRITEMASK);	// disable z-buffer writing

	const double Z_OFFSET = 20;

	glColor3d(1,1,1);
	glBegin(GL_LINES);
		glVertex3d(-1,0,-Z_OFFSET);		glVertex3d(1,0,-Z_OFFSET);
		glVertex3d(0,-1,-Z_OFFSET);		glVertex3d(0,1,-Z_OFFSET);
	glEnd();

	glPopAttrib();
	glPopMatrix();
}

void CmpModelRender::render_nuv_axes(const Vector3d& p, const Vector3d& n, const Vector3d& u, const Vector3d& v) const
{
	Vector3d pn = p + n;
	Vector3d pu = p + u;
	Vector3d pv = p + v;
	glBegin(GL_LINES);
		glColor3d(1,0,0);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pn.x, pn.y, pn.z);
		glColor3d(0,1,0);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pu.x, pu.y, pu.z);
		glColor3d(0,0,1);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pv.x, pv.y, pv.z);
	glEnd();
}

void CmpModelRender::update_active_item_hotspots(const ICmpUsable_Ptr& cmpItemUsable, const Skeleton_CPtr& itemSkeleton, const RBTMatrix_CPtr& characterMatrix) const
{
	const std::vector<std::string>& hotspots = cmpItemUsable->hotspots();
	for(size_t i=0, size=hotspots.size(); i<size; ++i)
	{
		const std::string& hotspot = hotspots[i];
		Bone_CPtr bone = itemSkeleton->bone_configuration()->bones(hotspot);

		// Calculate the hotspot position and orientation from those of the bone (in the model-local coordinate system).
		Vector3d orientation = characterMatrix->apply_to_vector(bone->orientation());
		Vector3d position = characterMatrix->apply_to_point(bone->position());

		cmpItemUsable->set_hotspot_orientation(hotspot, orientation);
		cmpItemUsable->set_hotspot_position(hotspot, position);
	}
}

}
