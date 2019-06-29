#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "HAL/EngineFwd.h"

#include "GameData/Core/ResourceHandle.h"

#include "HAL/Graphics/Texture.h"
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

		const Graphics::Texture& getTexture(ResourceHandle handle);
		const Graphics::Font& getFont(ResourceHandle handle);

		ResourceHandle lockTexture(const std::string& path);
		ResourceHandle lockFont(const std::string& path);

		void unlockResource(ResourceHandle handle);

	private:
		void createResourceLock(const std::string& path);

	private:
		std::unordered_map<ResourceHandle::IndexType, std::unique_ptr<Resource>> mResources;
		std::unordered_map<ResourceHandle::IndexType, int> mResourceLocksCount;
		std::unordered_map<std::string, ResourceHandle::IndexType> mPathsMap;
		std::map<ResourceHandle::IndexType, std::string> mPathFindMap;

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
