#pragma once

#include <string>
#include <map>

#include "Graphics/Texture.h"
#include "Graphics/Font.h"

#include <EngineFwd.h>

namespace SystemInterface
{
	/**
	 * Class that manage static resources such as textures
	 */
	class ResourceManager
	{
	public:
		ResourceManager(Engine* engine);
		~ResourceManager();

		Graphics::Texture getTexture(const std::string& texturePath);
		Graphics::Font getFont(const std::string& texturePath);
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
}
