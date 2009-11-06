/***
 * hesperus: SpriteManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SPRITEMANAGER
#define H_HESP_SPRITEMANAGER

#include <map>
#include <set>
#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Sprite> Sprite_Ptr;

// FIXME: SpriteManager works in essentially the same way as ModelManager. The commonality should be factored out.
class SpriteManager
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::map<std::string,Sprite_Ptr> m_sprites;

	//#################### PUBLIC METHODS ####################
public:
	void load_all();
	void register_sprite(const std::string& spriteName);
	const Sprite_Ptr& sprite(const std::string& spriteName);
	std::set<std::string> sprite_names() const;
};

}

#endif
