/***
 * hesperus: GUIComponent.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_GUICOMPONENT
#define H_HESP_GUICOMPONENT

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/util/UserInput.h>
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
	GUIComponent();

	//#################### DESTRUCTOR ####################
public:
	virtual ~GUIComponent();

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void render() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	const Extents& extents() const;
	virtual void fit(const Extents& extents, GUIComponent *parent);
	virtual void handle_input(UserInput& input);

	//#################### PROTECTED METHODS ####################
protected:
	void render_extents() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<GUIComponent> GUIComponent_Ptr;
typedef shared_ptr<const GUIComponent> GUIComponent_CPtr;

}

#endif
