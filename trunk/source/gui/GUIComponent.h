/***
 * hesperus: GUIComponent.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_GUICOMPONENT
#define H_HESP_GUICOMPONENT

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "Extents.h"

namespace hesp {

class GUIComponent
{
	//#################### PROTECTED VARIABLES ####################
protected:
	Extents_Ptr m_extents;
	GUIComponent *m_parent;

	//#################### CONSTRUCTORS ####################
public:
	GUIComponent() : m_parent(NULL) {}

	//#################### DESTRUCTOR ####################
public:
	virtual ~GUIComponent() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void render() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	const Extents& extents() const
	{
		return *m_extents;
	}

	virtual void fit(const Extents& extents, GUIComponent *parent)
	{
		m_extents = Extents_Ptr(new Extents(extents));
		m_parent = parent;
	}
};

//#################### TYPEDEFS ####################
typedef shared_ptr<GUIComponent> GUIComponent_Ptr;
typedef shared_ptr<const GUIComponent> GUIComponent_CPtr;

}

#endif
