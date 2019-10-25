#include "HAL/Base/ResourceManager.h"

#include <fstream>
#include <vector>
#include <experimental/filesystem>

#include <nlohmann/json.hpp>

#include "Debug/Log.h"
#include "Debug/Assert.h"

#include "HAL/Base/Engine.h"
#include "HAL/Internal/SdlSurface.h"

namespace HAL
{
	static Graphics::Sprite EMPTY_SPRITE = Graphics::Sprite();
	static Graphics::Texture EMPTY_TEXTURE = Graphics::Texture();
	static Graphics::Font EMPTY_FONT = Graphics::Font();
	static Graphics::SpriteAnimation EMPTY_SPRITE_ANIMATION = Graphics::SpriteAnimation();
	static Audio::Sound EMPTY_SOUND = Audio::Sound();
	static Audio::Music EMPTY_MUSIC = Audio::Music();

	ResourceManager::ResourceManager(Engine* engine)
		: mEngine(engine)
	{
	}

	template<>
	const Graphics::Texture& ResourceManager::getEmptyResource<Graphics::Texture>()
	{
		return EMPTY_TEXTURE;
	}

	template<>
	const Graphics::Sprite& ResourceManager::getEmptyResource<Graphics::Sprite>()
	{
		return EMPTY_SPRITE;
	}

	template<>
	const Graphics::SpriteAnimation& ResourceManager::getEmptyResource<Graphics::SpriteAnimation>()
	{
		return EMPTY_SPRITE_ANIMATION;
	}

	template<>
	const Graphics::Font& ResourceManager::getEmptyResource<Graphics::Font>()
	{
		return EMPTY_FONT;
	}

	template<>
	const Audio::Sound& ResourceManager::getEmptyResource<Audio::Sound>()
	{
		return EMPTY_SOUND;
	}

	template<>
	const Audio::Music& ResourceManager::getEmptyResource<Audio::Music>()
	{
		return EMPTY_MUSIC;
	}

	int ResourceManager::createResourceLock(const std::string& path)
	{
		mPathsMap[path] = mHandleIdx;
		mPathFindMap[mHandleIdx] = path;
		mResourceLocksCount[mHandleIdx] = 1;
		return mHandleIdx++;
	}

	void ResourceManager::loadOneAtlasData(const std::string& path)
	{
		namespace fs = std::experimental::filesystem;
		fs::path atlasDescPath(path);

		try
		{
			std::ifstream atlasDescriptionFile(atlasDescPath);
			nlohmann::json atlasJson;
			atlasDescriptionFile >> atlasJson;

			auto meta = atlasJson.at("meta");
			std::string atlasPath = meta.at("image");
			auto sizeJson = meta.at("size");
			Vector2D atlasSize;
			sizeJson.at("w").get_to(atlasSize.x);
			sizeJson.at("h").get_to(atlasSize.y);

			const auto& frames = atlasJson.at("frames");
			for (const auto& frameDataJson : frames)
			{
				AtlasFrameData frameData;
				std::string fileName = frameDataJson.at("filename");
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

	std::vector<std::string> ResourceManager::loadAnimData(const std::string& path)
	{
		namespace fs = std::experimental::filesystem;
		fs::path atlasDescPath(path);

		std::vector<std::string> result;
		std::string pathBase;
		int framesCount = 0;

		try
		{
			std::ifstream animDescriptionFile(atlasDescPath);
			nlohmann::json animJson;
			animDescriptionFile >> animJson;

			animJson.at("path").get_to(pathBase);
			animJson.at("frames").get_to(framesCount);

		}
		catch(const nlohmann::detail::exception& e)
		{
			LogError("Can't parse animation data '%s': %s", path.c_str(), e.what());
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

	ResourceHandle ResourceManager::lockTexture(const std::string& path)
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
			mResources[thisHandle] = std::make_unique<Graphics::Texture>(path, mEngine->getRenderer()->getRawRenderer());
			return ResourceHandle(thisHandle);
		}
	}

	ResourceHandle ResourceManager::lockFont(const std::string& path, int fontSize)
	{
		std::string id = path + ":" + std::to_string(fontSize);
		auto it = mPathsMap.find(id);
		if (it != mPathsMap.end())
		{
			++mResourceLocksCount[it->second];
			return ResourceHandle(it->second);
		}
		else
		{

			int thisHandle = createResourceLock(id);
			mResources[thisHandle] = std::make_unique<Graphics::Font>(path, fontSize, mEngine->getRenderer()->getRawRenderer());
			return ResourceHandle(thisHandle);
		}
	}

	ResourceHandle ResourceManager::lockSprite(const std::string& path)
	{
		std::string spritePathId = "spr-" + path;
		auto spritePathIt = mPathsMap.find(spritePathId);
		if (spritePathIt != mPathsMap.end())
		{
			++mResourceLocksCount[spritePathIt->second];
			return ResourceHandle(spritePathIt->second);
		}
		else
		{
			int thisHandle = createResourceLock(spritePathId);
			ResourceHandle originalTextureHandle;
			auto it = mAtlasFrames.find(path);
			if (it != mAtlasFrames.end())
			{
				originalTextureHandle = lockTexture(it->second.atlasPath);
				const Graphics::Texture& texture = getResource<Graphics::Texture>(originalTextureHandle);
				mResources[thisHandle] = std::make_unique<Graphics::Sprite>(&texture, it->second.quadUV);
			}
			else
			{
				originalTextureHandle = lockTexture(path);
				const Graphics::Texture& texture = getResource<Graphics::Texture>(originalTextureHandle);
				mResources[thisHandle] = std::make_unique<Graphics::Sprite>(&texture, Graphics::QuadUV());
			}
			return ResourceHandle(thisHandle);
		}
	}

	ResourceHandle ResourceManager::lockSound(const std::string& path)
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

	ResourceHandle ResourceManager::lockMusic(const std::string& path)
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

	ResourceHandle ResourceManager::lockSpriteAnimation(const std::string& path)
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
			std::vector<std::string> framePaths = loadAnimData(path);

			std::vector<ResourceHandle> frames;
			for (const auto& animFramePath : framePaths)
			{
				auto spriteHandle = lockSprite(animFramePath);
				frames.push_back(spriteHandle);
			}
			mResources[thisHandle] = std::make_unique<Graphics::SpriteAnimation>(std::move(frames));

			mResourceReleaseFns[thisHandle] = [this](Resource* resource)
			{
				for (auto& spriteHandle : static_cast<Graphics::SpriteAnimation*>(resource)->getSprites())
				{
					unlockResource(spriteHandle);
				}
			};

			return ResourceHandle(thisHandle);
		}
	}

	void ResourceManager::unlockResource(ResourceHandle handle)
	{
		auto locksCntIt = mResourceLocksCount.find(handle.ResourceIndex);
		AssertRetVoid(locksCntIt != mResourceLocksCount.end(), "Unlocking non-locked resource");
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

	void ResourceManager::loadAtlasesData(const std::string& listPath)
	{
		namespace fs = std::experimental::filesystem;
		fs::path listFsPath(listPath);

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
