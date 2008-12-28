/***
 * hesperus: PortalGenerator.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "PortalGenerator.h"

#include <source/exceptions/Exception.h>
#include <source/level/bsp/BSPBranch.h>
#include <source/level/bsp/BSPLeaf.h>
#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### PRIVATE METHODS ####################
/**
Clips the portal to the tree and returns a list of portal fragments which survive the clipping process.

@param portal	The portal to clip
@param tree		The tree to which to clip it
@return			The list of portal fragments which survived the clipping process
*/
std::list<Portal_Ptr> PortalGenerator::clip_portal_to_tree(const Portal_Ptr& portal, const BSPTree_Ptr& tree)
{
	return clip_portal_to_subtree(portal, tree->root()).first;
}

/**
Clips the portal to the subtree and returns a list of portal fragments which survive the clipping process.

@param portal				The portal to clip
@param subtreeRoot			The root of the subtree
@param relativeToPortal		The location of the subspace represented by the subtree relative to the portal (in front, behind, or straddling it)
@return						A pair, the first component of which is the list of portal fragments mentioned above,
							and the second of which is a boolean indicating whether the entirety of the initial
							portal survived the clipping process
*/
std::pair<std::list<Portal_Ptr>,bool> PortalGenerator::clip_portal_to_subtree(const Portal_Ptr& portal, const BSPNode_Ptr& subtreeRoot, PlaneClassifier relativeToPortal)
{
	if(subtreeRoot->is_leaf())
	{
		const BSPLeaf *leaf = subtreeRoot->as_leaf();

		if(leaf->is_solid()) return std::make_pair(std::list<Portal_Ptr>(), false);

		switch(relativeToPortal)
		{
			case CP_BACK:
			{
				portal->auxiliary_data().fromLeaf = leaf->leaf_index();
				break;
			}
			case CP_FRONT:
			{
				portal->auxiliary_data().toLeaf = leaf->leaf_index();
				break;
			}
			default:	// CP_STRADDLE (note that CP_COPLANAR is not possible here)
			{
				// The portal fragment is in the middle of a leaf (this is not an error, but we do need to
				// discard the portal fragment as we'd otherwise have a portal linking a leaf to itself).
				return std::make_pair(std::list<Portal_Ptr>(), false);
			}
		}
		std::list<Portal_Ptr> ret;
		ret.push_back(portal);
		return std::make_pair(ret, true);
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
				std::pair<std::list<Portal_Ptr>,bool> fromResult = clip_portal_to_subtree(portal, fromSubtree, CP_BACK);
				std::list<Portal_Ptr>& fromPortals = fromResult.first;
				if(fromResult.second)
				{
					// The entire portal survived whilst traversing the from subtree.
					return clip_portal_to_subtree(portal, toSubtree, CP_FRONT);
				}
				else
				{
					// The portal was clipped whilst traversing the from subtree, so all fragments need to be
					// clipped down the to subtree.
					std::list<Portal_Ptr> ret;
					for(std::list<Portal_Ptr>::const_iterator it=fromPortals.begin(), iend=fromPortals.end(); it!=iend; ++it)
					{
						ret.splice(ret.end(), clip_portal_to_subtree(*it, toSubtree, CP_FRONT).first);
					}
					return std::make_pair(ret, false);
				}
			}
			case CP_FRONT:
			{
				return clip_portal_to_subtree(portal, branch->left(), relativeToPortal);
			}
			case CP_STRADDLE:
			{
				// Note: The leaf links for the two half polygons are inherited from the original polygon here.
				SplitResults<Vector3d,PortalInfo> sr = split_polygon(*portal, *branch->splitter());

				std::list<Portal_Ptr> frontResult = clip_portal_to_subtree(sr.front, branch->left(), relativeToPortal).first;
				std::list<Portal_Ptr> backResult = clip_portal_to_subtree(sr.back, branch->right(), relativeToPortal).first;

				std::list<Portal_Ptr> ret;
				ret.splice(ret.end(), frontResult);
				ret.splice(ret.end(), backResult);
				return std::make_pair(ret, false);
			}
		}
	}

	// The code will never actually get here, because the switch above is exhaustive,
	// but the compiler still warns us because it can't tell that.
	throw Exception("This should never happen");
}

/**
Makes an initial portal on a given plane. This portal should be large enough to
span the entire level space.

@param plane	The plane on which to build the initial portal
@retun			As stated
*/
Portal_Ptr PortalGenerator::make_initial_portal(const Plane& plane)
{
	return make_universe_polygon<PortalInfo>(plane);
}

}
