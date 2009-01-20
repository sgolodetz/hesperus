/***
 * hesperus: LevelRenderer.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVELRENDERER
#define H_HESP_LEVELRENDERER

#include <map>
#include <set>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/textures/Texture.h>

namespace hesp {

class LevelRenderer
{
	//#################### PROTECTED VARIABLES ####################
protected:
	std::map<std::string,Texture_Ptr> m_textures;

	//#################### DESTRUCTOR ####################
public:
	virtual ~LevelRenderer() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void render(const std::vector<int>& polyIndices) const = 0;

	//#################### PROTECTED METHODS ####################
protected:
	void load_textures(const std::set<std::string>& textureNames);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<LevelRenderer> LevelRenderer_Ptr;
typedef shared_ptr<const LevelRenderer> LevelRenderer_CPtr;

}

#endif
