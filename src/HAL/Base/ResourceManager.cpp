#include "HAL/Base/ResourceManager.h"

#include <fstream>
#include <vector>
#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>

#include "Base/Debug/Log.h"
#include "Base/Debug/Assert.h"

#include "HAL/Base/Engine.h"
#include "HAL/Internal/SdlSurface.h"

#include "HAL/Audio/Music.h"
#include "HAL/Audio/Sound.h"
#include "HAL/Graphics/Font.h"
#include "HAL/Graphics/Sprite.h"
#include "HAL/Graphics/SpriteAnimationClip.h"
#include "HAL/Graphics/AnimationGroup.h"

namespace HAL
{
	ResourceManager::ResourceManager(Engine& engine)
		: mEngine(engine)
	{
	}

	int ResourceManager::createResourceLock(const ResourcePath& path)
	{
		mPathsMap[path] = mHandleIdx;
		mPathFindMap[mHandleIdx] = path;
		mResourceLocksCount[mHandleIdx] = 1;
		return mHandleIdx++;
	}

	void ResourceManager::loadOneAtlasData(const ResourcePath& path)
	{
		namespace fs = std::filesystem;
		fs::path atlasDescPath(static_cast<std::string>(path));

		try
		{
			std::ifstream atlasDescriptionFile(atlasDescPath);
			nlohmann::json atlasJson;
			atlasDescriptionFile >> atlasJson;

			auto meta = atlasJson.at("meta");
			ResourcePath atlasPath = meta.at("image");
			auto sizeJson = meta.at("size");
			Vector2D atlasSize;
			sizeJson.at("w").get_to(atlasSize.x);
			sizeJson.at("h").get_to(atlasSize.y);

			const auto& frames = atlasJson.at("frames");
			for (const auto& frameDataJson : frames)
			{
				AtlasFrameData frameData;
				ResourcePath fileName = frameDataJson.at("filename");
				auto frame = frameDataJson.at("frame");
				frameData.atlasPath = atlasPath;
				float x, y, w, h;
				frame.at("x").get_to(x);
				frame.at("y").get_to(y);
				frame.at("w").get_to(w);
				frame.at("h").get_to(h);

				frameData.quadUV.U1 = x / atlasSize.x;
				frameData.quadUV.V1 = y / atlasSize.y;
				frameData.quadUV.U2 = (x + w) / atlasSize.x;
				frameData.quadUV.V2 = (y + h) / atlasSize.y;
				mAtlasFrames.emplace(fileName, std::move(frameData));
			}
		}
		catch(const nlohmann::detail::exception& e)
		{
			LogError("Can't parse atlas data '%s': %s", path.c_str(), e.what());
		}
		catch(const std::exception& e)
		{
			LogError("Can't open atlas data '%s': %s", path.c_str(), e.what());
		}
	}

	std::vector<ResourcePath> ResourceManager::loadSpriteAnimClipData(const ResourcePath& path)
	{
		namespace fs = std::filesystem;
		fs::path atlasDescPath(static_cast<std::string>(path));

		std::vector<ResourcePath> result;
		ResourcePath pathBase;
		int framesCount = 0;

		try
		{
			std::ifstream animDescriptionFile(atlasDescPath);
			nlohmann::json animJson;
			animDescriptionFile >> animJson;

			animJson.at("path").get_to(pathBase);
			animJson.at("frames").get_to(framesCount);

		}
		catch(const std::exception& e)
		{
			LogError("Can't open animation data '%s': %s", path.c_str(), e.what());
		}

		for (int i = 0; i < framesCount; ++i)
		{
			result.emplace_back(pathBase + std::to_string(i) + ".png");
		}

		return result;
	}

	ResourceManager::AnimGroupData ResourceManager::loadAnimGroupData(const ResourcePath& path)
	{
		namespace fs = std::filesystem;
		fs::path atlasDescPath(static_cast<std::string>(path));

		AnimGroupData result;
		ResourcePath pathBase;

		try
		{
			std::ifstream animDescriptionFile(atlasDescPath);
			nlohmann::json animJson;
			animDescriptionFile >> animJson;

			animJson.at("clips").get_to(result.clips);
			animJson.at("stateMachineID").get_to(result.stateMachineID);
			animJson.at("defaultState").get_to(result.defaultState);
		}
		catch(const std::exception& e)
		{
			LogError("Can't open animation group data '%s': %s", path.c_str(), e.what());
		}

		return result;
	}

	ResourceHandle ResourceManager::lockSurface(const ResourcePath& path)
	{
		auto it = mPathsMap.find(path);
		if (it != mPathsMap.end())
		{
			++mResourceLocksCount[it->second];
			return ResourceHandle(it->second);
		}
		else
		{
			int thisHandle = createResourceLock(path);
			mResources[thisHandle] = std::make_unique<Graphics::Internal::Surface>(path);
			return ResourceHandle(thisHandle);
		}
	}

	ResourceHandle ResourceManager::lockFont(const ResourcePath& path, int fontSize)
	{
		std::string id = path + ":" + std::to_string(fontSize);
		auto it = mPathsMap.find(static_cast<ResourcePath>(id));
		if (it != mPathsMap.end())
		{
			++mResourceLocksCount[it->second];
			return ResourceHandle(it->second);
		}
		else
		{
			int thisHandle = createResourceLock(static_cast<ResourcePath>(id));
			mResources[thisHandle] = std::make_unique<Graphics::Font>(path, fontSize);
			return ResourceHandle(thisHandle);
		}
	}

	ResourceHandle ResourceManager::lockSprite(const ResourcePath& path)
	{
		std::string spritePathId = "spr-" + path;
		auto spritePathIt = mPathsMap.find(static_cast<ResourcePath>(spritePathId));
		if (spritePathIt != mPathsMap.end())
		{
			++mResourceLocksCount[spritePathIt->second];
			return ResourceHandle(spritePathIt->second);
		}
		else
		{
			int thisHandle = createResourceLock(static_cast<ResourcePath>(spritePathId));
			ResourceHandle originalSurfaceHandle;
			auto it = mAtlasFrames.find(path);
			if (it != mAtlasFrames.end())
			{
				originalSurfaceHandle = lockSurface(it->second.atlasPath);
				const Graphics::Internal::Surface& texture = getResource<Graphics::Internal::Surface>(originalSurfaceHandle);
				mResources[thisHandle] = std::make_unique<Graphics::Sprite>(&texture, it->second.quadUV);
			}
			else
			{
				originalSurfaceHandle = lockSurface(path);
				const Graphics::Internal::Surface& surface = getResource<Graphics::Internal::Surface>(originalSurfaceHandle);
				mResources[thisHandle] = std::make_unique<Graphics::Sprite>(&surface, Graphics::QuadUV());
			}
			return ResourceHandle(thisHandle);
		}
	}

	ResourceHandle ResourceManager::lockSound(const ResourcePath& path)
	{
		auto it = mPathsMap.find(path);
		if (it != mPathsMap.end())
		{
			++mResourceLocksCount[it->second];
			return ResourceHandle(it->second);
		}
		else
		{
			int thisHandle = createResourceLock(path);
			mResources[thisHandle] = std::make_unique<Audio::Sound>(path);
			return ResourceHandle(thisHandle);
		}
	}

	ResourceHandle ResourceManager::lockMusic(const ResourcePath& path)
	{
		auto it = mPathsMap.find(path);
		if (it != mPathsMap.end())
		{
			++mResourceLocksCount[it->second];
			return ResourceHandle(it->second);
		}
		else
		{
			int thisHandle = createResourceLock(path);
			mResources[thisHandle] = std::make_unique<Audio::Music>(path);
			return ResourceHandle(thisHandle);
		}
	}

	ResourceHandle ResourceManager::lockSpriteAnimationClip(const ResourcePath& path)
	{
		auto it = mPathsMap.find(path);
		if (it != mPathsMap.end())
		{
			++mResourceLocksCount[it->second];
			return ResourceHandle(it->second);
		}
		else
		{
			int thisHandle = createResourceLock(path);
			std::vector<ResourcePath> framePaths = loadSpriteAnimClipData(path);

			std::vector<ResourceHandle> frames;
			for (const auto& animFramePath : framePaths)
			{
				auto spriteHandle = lockSprite(animFramePath);
				frames.push_back(spriteHandle);
			}
			mResources[thisHandle] = std::make_unique<Graphics::SpriteAnimationClip>(std::move(frames));

			mResourceReleaseFns[thisHandle] = [this](Resource* resource)
			{
				for (auto& spriteHandle : static_cast<Graphics::SpriteAnimationClip*>(resource)->getSprites())
				{
					unlockResource(spriteHandle);
				}
			};

			return ResourceHandle(thisHandle);
		}
	}

	ResourceHandle ResourceManager::lockAnimationGroup(const ResourcePath& path)
	{
		auto it = mPathsMap.find(path);
		if (it != mPathsMap.end())
		{
			++mResourceLocksCount[it->second];
			return ResourceHandle(it->second);
		}
		else
		{
			int thisHandle = createResourceLock(path);
			AnimGroupData animGroupData = loadAnimGroupData(path);

			std::map<StringID, std::vector<ResourceHandle>> animClips;
			std::vector<ResourceHandle> clipsToRelease;
			clipsToRelease.reserve(animGroupData.clips.size());
			for (const auto& animClipPath : animGroupData.clips)
			{
				auto clipHandle = lockSpriteAnimationClip(animClipPath.second);
				animClips.emplace(animClipPath.first, getResource<Graphics::SpriteAnimationClip>(clipHandle).getSprites());
				clipsToRelease.push_back(clipHandle);
			}
			mResources[thisHandle] = std::make_unique<Graphics::AnimationGroup>(std::move(animClips), animGroupData.stateMachineID, animGroupData.defaultState);

			mResourceReleaseFns[thisHandle] = [this, clipsToRelease = std::move(clipsToRelease)](Resource*)
			{
				for (auto& animationClip : clipsToRelease)
				{
					unlockResource(animationClip);
				}
			};

			return ResourceHandle(thisHandle);
		}
	}

	void ResourceManager::unlockResource(ResourceHandle handle)
	{
		auto locksCntIt = mResourceLocksCount.find(handle.ResourceIndex);
		if ALMOST_NEVER(locksCntIt == mResourceLocksCount.end())
		{
			ReportError("Unlocking non-locked resource");
			return;
		}

		if (locksCntIt->second > 1)
		{
			--(locksCntIt->second);
			return;
		}
		else
		{
			// unload resource
			auto resourceIt = mResources.find(handle.ResourceIndex);
			if (resourceIt != mResources.end())
			{
				auto releaseFnIt = mResourceReleaseFns.find(handle.ResourceIndex);
				if (releaseFnIt != mResourceReleaseFns.end())
				{
					releaseFnIt->second(resourceIt->second.get());
					mResourceReleaseFns.erase(releaseFnIt);
				}
				mResources.erase(resourceIt);
			}
			mResourceLocksCount.erase(handle.ResourceIndex);
			auto pathIt = mPathFindMap.find(handle.ResourceIndex);
			if (pathIt != mPathFindMap.end())
			{
				mPathsMap.erase(pathIt->second);
			}
			mPathFindMap.erase(handle.ResourceIndex);
		}
	}

	void ResourceManager::loadAtlasesData(const ResourcePath& listPath)
	{
		namespace fs = std::filesystem;
		fs::path listFsPath(static_cast<std::string>(listPath));

		try
		{
			std::ifstream listFile(listFsPath);
			nlohmann::json listJson;
			listFile >> listJson;

			const auto& atlases = listJson.at("atlases");
			for (const auto& atlasPath : atlases.items())
			{
				loadOneAtlasData(atlasPath.value());
			}
		}
		catch(const nlohmann::detail::exception& e)
		{
			LogError("Can't parse atlas list '%s': %s", listPath.c_str(), e.what());
		}
		catch(const std::exception& e)
		{
			LogError("Can't open atlas list '%s': %s", listPath.c_str(), e.what());
		}
	}
}
