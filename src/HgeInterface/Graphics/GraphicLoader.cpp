#include "GraphicLoader.h"

#include <fstream>

#include <Debug/Log.h>

GraphicLoader::GraphicLoader()
{
	hge = nullptr;
}

GraphicLoader::~GraphicLoader()
{
	for (auto sprite : this->sprites)
	{
		delete sprite.second;
		sprite.second = nullptr;
	}

	for (auto texture : this->textures)
	{
		// ToDo: hge already destroyed it's will fail
		//this->hge->Texture_Free(texture.second);
		texture.second = 0;
	}

	Log::Instance().writeLog("GraphicLoader destroyed");
}

GraphicLoader& GraphicLoader::Instance()
{
	static GraphicLoader singleInstance;
	return singleInstance;
}

void GraphicLoader::init(HGE* hge, std::string graphicInfoFile, std::string imgFolder)
{
	if (this->hge != nullptr)
	{
		Log::Instance().writeWarning("GraphicLoader double initialization");
	}

	this->hge = hge;
	this->graphicInfoFileName = graphicInfoFile;
	this->imageFolder = imgFolder;
}

hgeSprite* GraphicLoader::getSprite(std::string spriteId)
{
	if (hge == nullptr)
	{
		std::string error = "GraphicLoader isn't initialised";
		Log::Instance().writeError(error);
		throw new std::runtime_error(error);
	}

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
		hgeSprite* sprite = nullptr;
		
		if (spriteInfo.loaded)
		{
			HTEXTURE texture = this->getTexture(spriteInfo.textureName);
			sprite = new hgeSprite(texture, (float)spriteInfo.posX, (float)spriteInfo.posY,
				(float)spriteInfo.sizeX, (float)spriteInfo.sizeY);
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

		// if we got required sprite
		graphicInfoFile >> spriteInfo.textureName;
		graphicInfoFile >> spriteInfo.posX;
		graphicInfoFile >> spriteInfo.posY;
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