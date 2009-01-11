/***
 * hesperus: FileSectionUtil_Save.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FileSectionUtil.h"

namespace hesp {

//#################### SAVING METHODS ####################
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

}