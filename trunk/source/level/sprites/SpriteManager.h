/***
 * hesperus: SpriteManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SPRITEMANAGER
#define H_HESP_SPRITEMANAGER

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/util/ResourceManager.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Sprite> Sprite_Ptr;

class SpriteManager : private ResourceManager<Sprite_Ptr>
{
	//#################### PUBLIC METHODS ####################
public:
	using ResourceManager<Sprite_Ptr>::load_all;
	void register_sprite(const std::string& spriteName);
	const Sprite_Ptr& sprite(const std::string& spriteName);
	std::set<std::string> sprite_names() const;

	//#################### PRIVATE METHODS ####################
private:
	Sprite_Ptr load_resource(const std::string& spriteName) const;
	std::string resource_type() const;
};

}

#endif
