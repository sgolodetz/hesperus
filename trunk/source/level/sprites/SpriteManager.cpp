/***
 * hesperus: SpriteManager.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "SpriteManager.h"

#include <boost/filesystem/operations.hpp>
namespace bf = boost::filesystem;

#include <source/exceptions/Exception.h>
#include <source/images/ImageLoader.h>
#include <source/io/util/DirectoryFinder.h>
#include <source/textures/TextureFactory.h>
#include "Sprite.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Loads all the registered sprites.
*/
void SpriteManager::load_all()
{
	for(std::map<std::string,Sprite_Ptr>::iterator it=m_sprites.begin(), iend=m_sprites.end(); it!=iend; ++it)
	{
		const std::string& name = it->first;
		Sprite_Ptr& sprite = it->second;

		if(!sprite)
		{
			bf::path imagesDir = determine_sprites_directory();
			std::string filename = (imagesDir / (name + ".png")).file_string();
			sprite.reset(new Sprite(TextureFactory::create_texture32(ImageLoader::load_image32(filename))));
		}
	}
}

/**
Registers a sprite for subsequent loading.

@param spriteName	The name of the sprite
*/
void SpriteManager::register_sprite(const std::string& spriteName)
{
	m_sprites.insert(std::make_pair(spriteName, Sprite_Ptr()));
}

/**
Returns the sprite with the specified name, if any.

@param spriteName	The name of the sprite
@return				The sprite, if it exists
@throw Exception	If the sprite doesn't exist
*/
const Sprite_Ptr& SpriteManager::sprite(const std::string& spriteName)
{
	std::map<std::string,Sprite_Ptr>::const_iterator it = m_sprites.find(spriteName);
	if(it != m_sprites.end()) return it->second;
	else throw Exception("This sprite manager does not contain a sprite named " + spriteName);
}

/**
Returns the names of the registered sprites.

@return	The registered sprite names
*/
std::set<std::string> SpriteManager::sprite_names() const
{
	std::set<std::string> ret;
	for(std::map<std::string,Sprite_Ptr>::const_iterator it=m_sprites.begin(), iend=m_sprites.end(); it!=iend; ++it)
	{
		ret.insert(it->first);
	}
	return ret;
}

}
