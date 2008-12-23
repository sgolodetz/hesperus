/***
 * hesperus: PortalGenerator.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "PortalGenerator.h"

#include <source/exceptions/Exception.h>
#include <source/math/geom/GeomUtil.h>
#include "BSPBranch.h"
#include "BSPLeaf.h"

namespace hesp {

//#################### PRIVATE METHODS ####################
/**
Clips the portal to the subtree and returns a list of portal fragments which survive the clipping process.

@param portal				The portal to clip
@param subtreeRoot			The root of the subtree
@param relativeToPortal		The location of the subspace represented by the subtree relative to the portal (in front, behind, or straddling it)
@return						The list of portal fragments mentioned above
*/
std::list<Portal_Ptr> PortalGenerator::clip_portal_to_subtree(const Portal_Ptr& portal, const BSPNode_Ptr& subtreeRoot, PlaneClassifier relativeToPortal)
{
	if(subtreeRoot->is_leaf())
	{
		const BSPLeaf *leaf = subtreeRoot->as_leaf();
		switch(relativeToPortal)
		{
			case CP_BACK:
			{
				portal->auxiliary_data().fromLeaf = leaf->index();
				break;
			}
			case CP_FRONT:
			{
				portal->auxiliary_data().toLeaf = leaf->index();
				break;
			}
			default:	// CP_STRADDLE (note that CP_COPLANAR is not possible here)
			{
				throw Exception("The portal fragment has somehow ended up in a leaf which is straddling it: oops!");
			}
		}
		std::list<Portal_Ptr> ret;
		ret.push_back(portal);
		return ret;
	}
	else
	{
		const BSPBranch *branch = subtreeRoot->as_branch();
		switch(classify_polygon_against_plane(*portal, *branch->splitter()))
		{
			case CP_BACK:
			{
				return clip_portal_to_subtree(portal, branch->right(), relativeToPortal);
			}
			case CP_COPLANAR:
			{
				BSPNode_Ptr fromSubtree;
				BSPNode_Ptr toSubtree;
				if(branch->splitter()->normal().dot(portal->normal()) > 0)
				{
					fromSubtree = branch->right();
					toSubtree = branch->left();
				}
				else
				{
					fromSubtree = branch->left();
					toSubtree = branch->right();
				}
				std::list<Portal_Ptr> fromPortals = clip_portal_to_subtree(portal, fromSubtree, CP_BACK);
				std::list<Portal_Ptr> ret;
				for(std::list<Portal_Ptr>::const_iterator it=fromPortals.begin(), iend=fromPortals.end(); it!=iend; ++it)
				{
					ret.splice(ret.end(), clip_portal_to_subtree(*it, toSubtree, CP_FRONT));
				}
				return ret;
			}
			case CP_FRONT:
			{
				return clip_portal_to_subtree(portal, branch->left(), relativeToPortal);
			}
			case CP_STRADDLE:
			{
				// Note: The leaf links for the two half polygons are inherited from the original polygon here.
				SplitResults<Portal::Vert,Portal::AuxData> sr = split_polygon(*portal, *branch->splitter());
				std::list<Portal_Ptr> ret = clip_portal_to_subtree(sr.front, branch->left(), relativeToPortal);
				ret.splice(ret.end(), clip_portal_to_subtree(sr.back, branch->right(), relativeToPortal));
				return ret;
			}
		}
	}

	// The code will never actually get here, because the switch above is exhaustive,
	// but the compiler still warns us because it can't tell that.
	throw Exception("This should never happen");
}

}
