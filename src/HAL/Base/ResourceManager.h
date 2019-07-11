#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "HAL/EngineFwd.h"

#include "GameData/Core/ResourceHandle.h"

#include "HAL/Graphics/Texture.h"
#include "HAL/Graphics/Sprite.h"
#include "HAL/Graphics/Font.h"

namespace HAL
{
	/**
	 * Class that manage static resources such as textures
	 */
	class ResourceManager
	{
	public:
		ResourceManager(Engine* engine);

		const Graphics::Sprite getSprite(ResourceHandle handle);
		ResourceHandle lockSprite(const std::string& path);

		const Graphics::Font& getFont(ResourceHandle handle);
		ResourceHandle lockFont(const std::string& path);

		void unlockResource(ResourceHandle handle);

		void loadAtlasesData(const std::string& listPath);

	private:
		struct AtlasFrameData
		{
			std::string atlasPath;
			Graphics::QuadUV quadUV;
		};

	private:
		void createResourceLock(const std::string& path);
		void loadOneAtlasData(const std::string& path);

		const Graphics::Texture& getTexture(ResourceHandle handle);
		ResourceHandle lockTexture(const std::string& path);

	private:
		std::unordered_map<ResourceHandle::IndexType, std::unique_ptr<Resource>> mResources;
		std::unordered_map<ResourceHandle::IndexType, int> mResourceLocksCount;
		std::unordered_map<std::string, ResourceHandle::IndexType> mPathsMap;
		std::map<ResourceHandle::IndexType, std::string> mPathFindMap;

		std::unordered_map<std::string, AtlasFrameData> mAtlasFrames;

		Engine* mEngine;

		std::string mGraphicInfoFileName;
		std::string mImageFolder;

		int mHandleIdx = 0;

		/*
		 * Turn off unusable operations
		 */
		ResourceManager(const ResourceManager&) = delete;
		void operator=(const ResourceManager&) = delete;
	};
}
