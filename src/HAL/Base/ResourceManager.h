#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <functional>

#include "Debug/Assert.h"

#include "GameData/Core/ResourceHandle.h"

#include "HAL/EngineFwd.h"

#include "HAL/Audio/Music.h"
#include "HAL/Audio/Sound.h"
#include "HAL/Graphics/Font.h"
#include "HAL/Graphics/Texture.h"
#include "HAL/Graphics/Sprite.h"
#include "HAL/Graphics/SpriteAnimation.h"

namespace HAL
{
	/**
	 * Class that manage static resources such as textures
	 */
	class ResourceManager
	{
	public:
		explicit ResourceManager(Engine* engine);

		ResourceHandle lockFont(const std::string& path, int fontSize);
		ResourceHandle lockTexture(const std::string& path);
		ResourceHandle lockSprite(const std::string& path);
		ResourceHandle lockSpriteAnimation(const std::string& path);
		ResourceHandle lockSound(const std::string& path);
		ResourceHandle lockMusic(const std::string& path);

		template<typename T>
		const T& getResource(ResourceHandle handle)
		{
			auto it = mResources.find(handle.ResourceIndex);
			AssertRet(it != mResources.end(), getEmptyResource<T>(), "Trying to access non loaded resource");
			return static_cast<T&>(*(it->second.get()));
		}

		void unlockResource(ResourceHandle handle);

		void loadAtlasesData(const std::string& listPath);

	private:
		struct AtlasFrameData
		{
			std::string atlasPath;
			Graphics::QuadUV quadUV;
		};

		struct AnimationData
		{
			std::vector<std::string> framePaths;
			int framesCount = 0;
		};

		using ReleaseFn = std::function<void(Resource*)>;

	private:
		template<typename T>
		const T& getEmptyResource();

		void createResourceLock(const std::string& path);

		void loadOneAtlasData(const std::string& path);

	private:
		std::unordered_map<ResourceHandle::IndexType, std::unique_ptr<Resource>> mResources;
		std::unordered_map<ResourceHandle::IndexType, int> mResourceLocksCount;
		std::unordered_map<ResourceHandle::IndexType, ReleaseFn> mResourceReleaseFns;
		std::unordered_map<std::string, ResourceHandle::IndexType> mPathsMap;
		std::map<ResourceHandle::IndexType, std::string> mPathFindMap;

		std::unordered_map<std::string, AtlasFrameData> mAtlasFrames;

		std::unordered_map<std::string, AnimationData> mAnimationData;

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
