/***
 * hesperus: SpriteManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SPRITEMANAGER
#define H_HESP_SPRITEMANAGER

#include <source/util/ResourceManager.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Sprite> Sprite_Ptr;
typedef shared_ptr<const class Sprite> Sprite_CPtr;

class SpriteManager : public ResourceManager<Sprite>
{
	//#################### PUBLIC METHODS ####################
public:
	void register_sprite(const std::string& spriteName);
	const Sprite_Ptr& sprite(const std::string& spriteName);
	Sprite_CPtr sprite(const std::string& spriteName) const;
	std::set<std::string> sprite_names() const;

	//#################### PRIVATE METHODS ####################
private:
	Sprite_Ptr load_resource(const std::string& spriteName) const;
	std::string resource_type() const;
};

}

#endif
