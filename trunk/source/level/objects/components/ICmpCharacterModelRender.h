/***
 * hesperus: ICmpCharacterModelRender.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPCHARACTERMODELRENDER
#define H_HESP_ICMPCHARACTERMODELRENDER

#include "ICmpModelRender.h"

namespace hesp {

class ICmpCharacterModelRender : public ICmpModelRender
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void render_first_person() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string own_type() const			{ return "CharacterModelRender"; }
	static std::string static_own_type()	{ return "CharacterModelRender"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpCharacterModelRender> ICmpCharacterModelRender_Ptr;
typedef shared_ptr<const ICmpCharacterModelRender> ICmpCharacterModelRender_CPtr;

}

#endif
