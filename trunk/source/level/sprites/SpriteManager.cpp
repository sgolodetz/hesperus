/***
 * hesperus: SpriteManager.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "SpriteManager.h"

#include <boost/filesystem/operations.hpp>
namespace bf = boost::filesystem;

#include <source/images/ImageLoader.h>
#include <source/io/util/DirectoryFinder.h>
#include <source/textures/TextureFactory.h>
#include "Sprite.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
void SpriteManager::register_sprite(const std::string& spriteName)		{ register_resource(spriteName); }
const Sprite_Ptr& SpriteManager::sprite(const std::string& spriteName)	{ return resource(spriteName); }
std::set<std::string> SpriteManager::sprite_names() const				{ return resource_names(); }

//#################### PRIVATE METHODS ####################
Sprite_Ptr SpriteManager::load_resource(const std::string& spriteName) const
{
	bf::path spritesDir = determine_sprites_directory();
	std::string filename = (spritesDir / (spriteName + ".png")).file_string();
	return Sprite_Ptr(new Sprite(TextureFactory::create_texture32(ImageLoader::load_image32(filename))));
}

std::string SpriteManager::resource_type() const
{
	return "sprite";
}

}
