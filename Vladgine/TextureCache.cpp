#include "TextureCache.h"
#include "IMGLoader.h"
#include <iostream>

namespace Vladgine {

	TextureCache::TextureCache()
	{
	}

	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(std::string texturePath)
	{
		//look up a texture if its in the map 
		auto mit = _textureMap.find(texturePath);

		//check if o\its not in the map
		if (mit == _textureMap.end()) {
			// make new texture
			GLTexture newTexture = IMGLoader::loadPNG(texturePath);

			//insert the pair of string(key) and GLTexture(value) in the map
			_textureMap.insert(make_pair(texturePath, newTexture));

			std::cout << "Loaded  Texture!\n";

			return newTexture;
		}


		std::cout << "Used Cached Texture!\n";

		return mit->second;

	}

}