/***
 * hesperus: FileSectionUtil_Save.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FileSectionUtil.h"

#include <source/images/BitmapSaver.h>

namespace hesp {

//#################### SAVING METHODS ####################
/**
Saves a set of entities to the specified std::ostream.

@param os				The std::ostream
@param entityManager	An EntityManager containing the entities to save
*/
void FileSectionUtil::save_entities_section(std::ostream& os, const EntityManager_Ptr& entityManager)
{
	os << "Entities\n";
	os << "{\n";

	os << "DefinitionFile\n";
	os << "{\n";
	os << entityManager->definition_filename() << '\n';
	os << "}\n";

	os << "Instances\n";
	os << "{\n";

	const std::vector<Entity_Ptr>& entities = entityManager->entities();
	int entityCount = static_cast<int>(entities.size());
	os << entityCount << '\n';
	for(int i=0; i<entityCount; ++i)
	{
		entities[i]->save(os);
	}

	os << "}\n";

	os << "}\n";
}

/**
Saves a lightmap prefix to the specified std::ostream.

@param os	The std::ostream
@param		The lightmap prefix
*/
void FileSectionUtil::save_lightmap_prefix_section(std::ostream& os, const std::string& lightmapPrefix)
{
	os << "LightmapPrefix\n";
	os << "{\n";
	os << lightmapPrefix << '\n';
	os << "}\n";
}

/**
Saves an array of lightmaps to the specified std::ostream.

@param os			The std::ostream
@param lightmaps	The lightmaps
*/
void FileSectionUtil::save_lightmaps_section(std::ostream& os, const std::vector<Image24_Ptr>& lightmaps)
{
	os << "Lightmaps\n";
	os << "{\n";

	int lightmapCount = static_cast<int>(lightmaps.size());
	os << lightmapCount << '\n';

	for(int i=0; i<lightmapCount; ++i)
	{
		BitmapSaver::save_image24(os, lightmaps[i]);
	}

	os << "\n}\n";
}

/**
Saves an array of lights to the specified std::ostream.

@param os		The std::ostream
@param lights	The lights
*/
void FileSectionUtil::save_lights_section(std::ostream& os, const std::vector<Light>& lights)
{
	os << "Lights\n";
	os << "{\n";

	int lightCount = static_cast<int>(lights.size());
	os << lightCount << '\n';

	for(int i=0; i<lightCount; ++i)
	{
		os << lights[i].position << ' ' << lights[i].colour << ' ' << lights[i].falloffRadius << '\n';
	}

	os << "}\n";
}

/**
Saves an array of navigation datasets to the specified std::ostream.

@param os			The std::ostream
@param datasets		The navigation datasets
*/
void FileSectionUtil::save_nav_section(std::ostream& os, const std::vector<NavDataset_Ptr>& datasets)
{
	os << "Nav\n";
	os << "{\n";

	// Output the datasets sequentially.
	int datasetCount = static_cast<int>(datasets.size());
	os << datasetCount << '\n';
	for(int i=0; i<datasetCount; ++i)
	{
		os << "Dataset\n";
		os << "{\n";

		write_navmesh(os, datasets[i]->nav_mesh());
		write_adjacency_list(os, datasets[i]->adjacency_list());
		write_path_table(os, datasets[i]->path_table());

		os << "}\n";
	}

	os << "}\n";
}

/**
Saves an onion tree to the specified std::ostream.

@param os		The std::ostream
@param tree		The onion tree
*/
void FileSectionUtil::save_onion_tree_section(std::ostream& os, const OnionTree_Ptr& tree)
{
	os << "OnionTree\n";
	os << "{\n";
	tree->output_postorder_text(os);
	os << "}\n";
}

/**
Saves a BSP tree to the specified std::ostream.

@param os		The std::ostream
@param tree		The BSP tree
*/
void FileSectionUtil::save_tree_section(std::ostream& os, const BSPTree_Ptr& tree)
{
	os << "BSPTree\n";
	os << "{\n";
	tree->output_postorder_text(os);
	os << "}\n";
}

/**
Saves a leaf visibility table to a std::ostream.

@param os		The std::ostream
@param leafVis	The leaf visibility table
*/
void FileSectionUtil::save_vis_section(std::ostream& os, const LeafVisTable_Ptr& leafVis)
{
	const LeafVisTable& table = *leafVis;

	os << "VisTable\n";
	os << "{\n";

	int size = table.size();
	os << size << '\n';
	for(int i=0; i<size; ++i)
	{
		for(int j=0; j<size; ++j)
		{
			switch(table(i,j))
			{
				case LEAFVIS_NO:
				{
					os << '0';
					break;
				}
				case LEAFVIS_YES:
				{
					os << '1';
					break;
				}
			}
		}
		os << '\n';
	}

	os << "}\n";
}

//#################### SAVING SUPPORT METHODS ####################
/**
Writes an adjacency list to the specified std::ostream.
*/
void FileSectionUtil::write_adjacency_list(std::ostream& os, const AdjacencyList_Ptr& adjList)
{
	os << "AdjacencyList\n";
	os << "{\n";

	int size = adjList->size();
	os << size << '\n';
	for(int i=0; i<size; ++i)
	{
		os << i << ' ';
		const std::list<AdjacencyList::Edge>& adjEdges = adjList->adjacent_edges(i);
		for(std::list<AdjacencyList::Edge>::const_iterator jt=adjEdges.begin(), jend=adjEdges.end(); jt!=jend;)
		{
			os << "( " << jt->to_node() << ' ' << jt->length() << " )";
			++jt;
			if(jt != jend) os << ' ';
		}
		os << '\n';
	}

	os << "}\n";
}

/**
Writes a navigation mesh to the specified std::ostream.
*/
void FileSectionUtil::write_navmesh(std::ostream& os, const NavMesh_Ptr& mesh)
{
	os << "Mesh\n";
	os << "{\n";

	// Output the nav links.
	os << "Links\n";
	os << "{\n";
	const std::vector<NavLink_Ptr>& links = mesh->links();
	int linkCount = static_cast<int>(links.size());
	for(int i=0; i<linkCount; ++i)
	{
		links[i]->output(os);
		os << '\n';
	}
	os << "}\n";

	// Output the polygons.
	os << "Polygons\n";
	os << "{\n";
	const std::vector<NavPolygon_Ptr>& polygons = mesh->polygons();
	int polyCount = static_cast<int>(polygons.size());
	for(int i=0; i<polyCount; ++i)
	{
		os << i << ' ' << polygons[i]->collision_poly_index();

		// Output the in links for this polygon.
		os << " [ ";
		const std::vector<int>& inLinks = polygons[i]->in_links();
		size_t inLinkCount = inLinks.size();
		for(size_t j=0; j<inLinkCount; ++j) os << inLinks[j] << ' ';
		os << "]";

		// Output the out links for this polygon.
		os << " [ ";
		const std::vector<int>& outLinks = polygons[i]->out_links();
		size_t outLinkCount = outLinks.size();
		for(size_t j=0; j<outLinkCount; ++j) os << outLinks[j] << ' ';
		os << "]\n";

	}
	os << "}\n";

	os << "}\n";
}

/**
Writes a path table to the specified std::ostream in binary format.
*/
void FileSectionUtil::write_path_table(std::ostream& os, const PathTable_Ptr& pathTable)
{
	os << "PathTable\n";
	os << "{\n";

	int size = pathTable->size();
	os << size << '\n';

	for(int i=0; i<size; ++i)
		for(int j=0; j<size; ++j)
		{
			// TODO: There may be endian issues with this if we ever port to another platform.
			os.write(reinterpret_cast<const char*>(&pathTable->next_node(i,j)), sizeof(int));
			os.write(reinterpret_cast<const char*>(&pathTable->cost(i,j)), sizeof(float));
		}

	os << "\n}\n";
}

}
