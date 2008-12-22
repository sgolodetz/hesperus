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
	std::list<Portal_Ptr> ret;

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
		ret.push_back(portal);
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
				// TODO
				break;
			}
			case CP_FRONT:
			{
				return clip_portal_to_subtree(portal, branch->left(), relativeToPortal);
			}
			case CP_STRADDLE:
			{
				// TODO
				break;
			}
		}
	}

	return ret;
}

}
