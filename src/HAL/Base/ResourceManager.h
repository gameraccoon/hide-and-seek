#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <functional>

#include "Debug/Assert.h"

#include "GameData/Core/ResourceHandle.h"

#include "HAL/Base/Resource.h"
#include "HAL/Base/Types.h"
#include "HAL/EngineFwd.h"

namespace HAL
{
	/**
	 * Class that manage static resources such as textures
	 */
	class ResourceManager
	{
	public:
		explicit ResourceManager(Engine& engine);

		~ResourceManager() = default;

		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;

		ResourceHandle lockFont(const std::string& path, int fontSize);
		ResourceHandle lockTexture(const std::string& path);
		ResourceHandle lockSprite(const std::string& path);
		ResourceHandle lockSpriteAnimationClip(const std::string& path);
		ResourceHandle lockAnimationGroup(const std::string& path);
		ResourceHandle lockSound(const std::string& path);
		ResourceHandle lockMusic(const std::string& path);

		template<typename T>
		[[nodiscard]] const T& getResource(ResourceHandle handle)
		{
			auto it = mResources.find(handle.ResourceIndex);
			AssertFatal(it != mResources.end(), "Trying to access non loaded resource");
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

		struct AnimGroupData
		{
			std::map<std::string, std::string> clips;
			std::string stateMachineID;
			std::string defaultState;
		};

		using ReleaseFn = std::function<void(Resource*)>;

	private:
		int createResourceLock(const std::string& path);

		void loadOneAtlasData(const std::string& path);
		std::vector<std::string> loadSpriteAnimClipData(const std::string& path);
		AnimGroupData loadAnimGroupData(const std::string& path);

	private:
		std::unordered_map<ResourceHandle::IndexType, std::unique_ptr<Resource>> mResources;
		std::unordered_map<ResourceHandle::IndexType, int> mResourceLocksCount;
		std::unordered_map<ResourceHandle::IndexType, ReleaseFn> mResourceReleaseFns;
		std::unordered_map<std::string, ResourceHandle::IndexType> mPathsMap;
		std::map<ResourceHandle::IndexType, std::string> mPathFindMap;

		std::unordered_map<std::string, AtlasFrameData> mAtlasFrames;

		Engine& mEngine;

		std::string mGraphicInfoFileName;
		std::string mImageFolder;

		int mHandleIdx = 0;
	};
}
