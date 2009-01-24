/***
 * hesperus: LevelFileUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVELFILEUTIL
#define H_HESP_LEVELFILEUTIL

#include <source/images/Image.h>
#include <source/level/Level.h>
#include <source/util/PolygonTypes.h>

namespace hesp {

class LevelFileUtil
{
	//#################### LOADING METHODS ####################
public:
	static Level_Ptr load(const std::string& filename);

	//#################### SAVING METHODS ####################
public:
	static void save_lit(const std::string& filename,
						 const std::vector<TexturedLitPolygon_Ptr>& polygons, const BSPTree_Ptr& tree,
						 const std::vector<Portal_Ptr>& portals,
						 const LeafVisTable_Ptr& leafVis,
						 const std::vector<Image24_Ptr>& lightmaps,
						 const std::vector<CollisionPolygon_Ptr>& onionPolygons, const OnionTree_Ptr& onionTree,
						 const std::vector<OnionPortal_Ptr>& onionPortals,
						 const std::vector<NavDataset_Ptr>& navDatasets);
	static void save_unlit(const std::string& filename,
						   const std::vector<TexturedPolygon_Ptr>& polygons, const BSPTree_Ptr& tree,
						   const std::vector<Portal_Ptr>& portals,
						   const LeafVisTable_Ptr& leafVis,
						   const std::vector<CollisionPolygon_Ptr>& onionPolygons, const OnionTree_Ptr& onionTree,
						   const std::vector<OnionPortal_Ptr>& onionPortals,
						   const std::vector<NavDataset_Ptr>& navDatasets);

	//#################### LOADING SUPPORT METHODS ####################
private:
	static Level_Ptr load_lit(std::istream& is);
	static Level_Ptr load_unlit(std::istream& is);
};

}

#endif
