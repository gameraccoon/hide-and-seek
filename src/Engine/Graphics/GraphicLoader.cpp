#include "GraphicLoader.h"

#include <fstream>

// dummy for debugging methods
#if (!defined DEBUG) && (!defined RELEASE)
 	#define RELEASE
  	#define WARN(message)
  	#define WARN_IF(condition, message)
#endif

GraphicLoader::GraphicLoader(HGE* hge, std::string graphicInfoFile, std::string imgFolder)
{
	this->hge = hge;
	this->graphicInfoFileName = graphicInfoFile;
	this->imageFolder = imgFolder;
}


GraphicLoader::~GraphicLoader(void)
{
}

hgeSprite* GraphicLoader::getSprite(std::string classId)
{
	std::string spriteId;
	spriteId = classId;

	SpriteMap::iterator it = this->sprites.find(spriteId);
	if (it != sprites.end())
	{ // if we have this sprite in map
		return it->second;
	}
	else
	{
		// get sprite info from outer source
		SpriteInfo spriteInfo = this->loadSpriteInfo(spriteId);
		// get existed texture or load from file
		hgeSprite* sprite = NULL;
		
		if (spriteInfo.loaded)
		{
			HTEXTURE texture = this->getTexture(spriteInfo.textureName);
			sprite = new hgeSprite(texture, 0, 0, (float)spriteInfo.sizeX, (float)spriteInfo.sizeY);
			sprite->SetColor(spriteInfo.color);
			sprite->SetHotSpot((float)spriteInfo.hotSpotX, (float)spriteInfo.hotSpotY);
		}

		sprites.insert(SpriteMap::value_type(spriteId, sprite));

		return sprite;
	}
}

GraphicLoader::SpriteInfo GraphicLoader::loadSpriteInfo(std::string id)
{
	SpriteInfo spriteInfo;
	spriteInfo.loaded = false;

	std::ifstream graphicInfoFile;
	try
	{
		graphicInfoFile.open(graphicInfoFileName);
	}
	catch(std::exception)
	{
		return spriteInfo;
	}

	while (!graphicInfoFile.eof())
	{
		std::string className;
		graphicInfoFile >> className; // get from filestream
		
		if (className.compare(std::string("[").append(id).append("]")) != 0)
			continue;

		// if we got required class
		graphicInfoFile >> spriteInfo.textureName;
		graphicInfoFile >> spriteInfo.sizeX;
		graphicInfoFile >> spriteInfo.sizeY;
		graphicInfoFile >> spriteInfo.hotSpotX;
		graphicInfoFile >> spriteInfo.hotSpotY;
		graphicInfoFile >> spriteInfo.color;

		std::string endDelimeter;
		graphicInfoFile >> endDelimeter;

		spriteInfo.loaded = endDelimeter.compare("end") == 0;

		break;
	}
	graphicInfoFile.close();

	return spriteInfo;
}

HTEXTURE GraphicLoader::getTexture(std::string name)
{
	TextureMap::iterator it = this->textures.find(name);
	if (it != textures.end())
	{ // if we already load the texture
		return it->second;
	}
	else
	{
		HTEXTURE texture = this->hge->Texture_Load(std::string(this->imageFolder).append(name).c_str());

		if (!texture)
		{
			throw new TextureNotFoundException();
		}

		textures.insert(TextureMap::value_type(name, texture));

		return texture;
	}
}