#pragma once

#include <string>
#include <map>

#include "Texture.h"
#include "Font.h"

#include <EngineInterface/EngineFwd.h>

/**
 *
 */
class ResourceManager
{
public:
	ResourceManager(Engine::Internal::Engine* engine);
	~ResourceManager();

	Texture getTexture(const std::string& texturePath);
	Font getFont(const std::string& texturePath);
private:
	// types
	struct SpriteInfo
	{
		std::string id;
		std::string textureName;
		int posX, posY;
		int sizeX, sizeY;
		//DWORD color;
		int hotSpotX, hotSpotY;
		bool loaded;
	};

	class Impl;
	std::unique_ptr<Impl> mPimpl;

	std::string mGraphicInfoFileName;
	std::string mImageFolder;

	// methods
	SpriteInfo loadSpriteInfo(const std::string& id);

	/*
	 * Turn off unusable operations
	 */
	ResourceManager(const ResourceManager&) = delete;
	void operator=(const ResourceManager&) = delete;
};
