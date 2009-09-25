/***
 * hesperus: DefinitionsFile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "DefinitionsFile.h"

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>
#include <source/level/collisions/AABBBounds.h>
#include <source/level/collisions/BoundsManager.h>
#include <source/level/collisions/PointBounds.h>
#include <source/level/collisions/SphereBounds.h>
#include <source/xml/XMLParser.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads the specified definitions file.

@param filename					The name of the file
@param boundsManager			Used to return the various bounds for objects
@param componentPropertyTypes	Used to return the types of the properties in the various object components
*/
void DefinitionsFile::load(const std::string& filename, BoundsManager_Ptr& boundsManager,
						   std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes)
{
	XMLLexer_Ptr lexer(new XMLLexer(filename));
	XMLParser parser(lexer);
	XMLElement_CPtr root = parser.parse();
	XMLElement_CPtr definitionsElt = root->find_unique_child("definitions");

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Process the objects subtree.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	{
		XMLElement_CPtr objectsElt = definitionsElt->find_unique_child("objects");

		// Load the object bounds.
		XMLElement_CPtr boundsElt = objectsElt->find_unique_child("bounds");
		boundsManager = load_bounds(boundsElt);

		// Load the component property types.
		XMLElement_CPtr componentsElt = objectsElt->find_unique_child("components");
		std::vector<XMLElement_CPtr> componentElts = componentsElt->find_children("component");

		for(size_t i=0, componentCount=componentElts.size(); i<componentCount; ++i)
		{
			const XMLElement_CPtr& componentElt = componentElts[i];
			const std::string& componentName = componentElt->attribute("name");
			std::map<std::string,std::string>& propertyTypes = componentPropertyTypes[componentName];

			// Load the property types for component i.
			std::vector<XMLElement_CPtr> propertyElts = componentElt->find_children("property");
			for(size_t j=0, propertyCount=propertyElts.size(); j<propertyCount; ++j)
			{
				const XMLElement_CPtr& propertyElt = propertyElts[j];
				const std::string& name = propertyElt->attribute("name");
				std::string type = propertyElt->attribute("type");
				propertyTypes.insert(std::make_pair(name, type));
			}
		}

		// TODO: Process any other subtrees when it becomes necessary.
	}

	// TODO: Process any other subtrees when it becomes necessary.
}

/**
Loads the bounds from the specified definitions file.

@param filename	The name of the file from which to load the bounds
@return			The bounds
*/
BoundsManager_Ptr DefinitionsFile::load_bounds_only(const std::string& filename)
{
	XMLLexer_Ptr lexer(new XMLLexer(filename));
	XMLParser parser(lexer);
	XMLElement_CPtr root = parser.parse();
	XMLElement_CPtr definitionsElt = root->find_unique_child("definitions");
	XMLElement_CPtr objectsElt = definitionsElt->find_unique_child("objects");
	XMLElement_CPtr boundsElt = objectsElt->find_unique_child("bounds");
	return load_bounds(boundsElt);
}

//#################### LOADING SUPPORT METHODS ####################
Bounds_Ptr DefinitionsFile::load_aabb_bounds(const XMLElement_CPtr& elt)
{
	double sx = lexical_cast<double,std::string>(elt->attribute("sx"));
	double sy = lexical_cast<double,std::string>(elt->attribute("sy"));
	double sz = lexical_cast<double,std::string>(elt->attribute("sz"));
	return Bounds_Ptr(new AABBBounds(Vector3d(sx,sy,sz)));
}

BoundsManager_Ptr DefinitionsFile::load_bounds(const XMLElement_CPtr& boundsElt)
{
	typedef BoundsManager::BoundsGroup BoundsGroup;
	std::vector<Bounds_CPtr> bounds;
	std::map<std::string,int> boundsLookup;
	std::map<std::string,BoundsGroup> boundsGroups;

	// Load the bounds.
	typedef Bounds_Ptr(*BoundsLoader)(const XMLElement_CPtr&);
	typedef std::map<std::string,BoundsLoader> BoundsLoaders;
	BoundsLoaders boundsLoaders;
	boundsLoaders["aabb"]	= load_aabb_bounds;
	boundsLoaders["point"]	= load_point_bounds;
	boundsLoaders["sphere"]	= load_sphere_bounds;

	std::vector<XMLElement_CPtr> boundElts = boundsElt->find_children("bound");
	for(size_t i=0, size=boundElts.size(); i<size; ++i)
	{
		const XMLElement_CPtr& boundElt = boundElts[i];
		const std::string& boundName = boundElt->attribute("name");
		const std::string& boundType = boundElt->attribute("type");

		BoundsLoaders::const_iterator jt = boundsLoaders.find(boundType);
		if(jt != boundsLoaders.end())
		{
			BoundsLoader loader = jt->second;
			boundsLookup.insert(std::make_pair(boundName, static_cast<int>(bounds.size())));
			bounds.push_back(loader(boundElt));
		}
		else throw Exception("No such bounds type: " + boundType);
	}

	// Load the groups.
	std::vector<XMLElement_CPtr> groupElts = boundsElt->find_children("group");
	for(size_t i=0, groupCount=groupElts.size(); i<groupCount; ++i)
	{
		const XMLElement_CPtr& groupElt = groupElts[i];
		const std::string& groupName = groupElt->attribute("name");

		std::vector<XMLElement_CPtr> postureElts = groupElt->find_children("posture");
		for(size_t j=0, postureCount=postureElts.size(); j<postureCount; ++j)
		{
			const XMLElement_CPtr& postureElt = postureElts[j];
			const std::string& postureName = postureElt->attribute("name");
			const std::string& postureBound = postureElt->attribute("bound");
			boundsGroups[groupName].insert(std::make_pair(postureName, postureBound));
		}
	}

	return BoundsManager_Ptr(new BoundsManager(bounds, boundsLookup, boundsGroups));
}

Bounds_Ptr DefinitionsFile::load_point_bounds(const XMLElement_CPtr&)
{
	return Bounds_Ptr(new PointBounds);
}

Bounds_Ptr DefinitionsFile::load_sphere_bounds(const XMLElement_CPtr& elt)
{
	double radius = lexical_cast<double,std::string>(elt->attribute("radius"));
	return Bounds_Ptr(new SphereBounds(radius));
}

}
