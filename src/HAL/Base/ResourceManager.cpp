#include "HAL/Base/ResourceManager.h"

#include <fstream>
#include <vector>
#include <experimental/filesystem>

#include <nlohmann/json.hpp>

#include "Debug/Log.h"
#include "Debug/Assert.h"

#include "HAL/Base/Engine.h"
#include "../Internal/SdlSurface.h"

namespace HAL
{
	static Graphics::Texture EMPTY_TEXTURE = Graphics::Texture(nullptr);
	static Graphics::Sprite EMPTY_SPRITE = Graphics::Sprite(nullptr, Graphics::QuadUV());
	static Graphics::SpriteAnimation EMPTY_SPRITE_ANIMATION = Graphics::SpriteAnimation(std::vector<ResourceHandle>());
	static Graphics::Font EMPTY_FONT = Graphics::Font(nullptr);

	ResourceManager::ResourceManager(Engine* engine)
		: mEngine(engine)
	{
	}

	const Graphics::Texture& ResourceManager::getTexture(ResourceHandle handle)
	{
		auto it = mResources.find(handle.ResourceIndex);
		AssertRet(it != mResources.end(), EMPTY_TEXTURE, "Trying to access non loaded texture");
		return static_cast<Graphics::Texture&>(*(it->second.get()));
	}

	const Graphics::Sprite& ResourceManager::getSprite(ResourceHandle handle)
	{
		auto it = mResources.find(handle.ResourceIndex);
		AssertRet(it != mResources.end(), EMPTY_SPRITE, "Trying to access non loaded sprite");
		return static_cast<Graphics::Sprite&>(*(it->second.get()));
	}

	const Graphics::SpriteAnimation& ResourceManager::getSpriteAnimation(ResourceHandle handle)
	{
		auto it = mResources.find(handle.ResourceIndex);
		AssertRet(it != mResources.end(), EMPTY_SPRITE_ANIMATION, "Trying to access non loaded sprite animation");
		return static_cast<Graphics::SpriteAnimation&>(*(it->second.get()));
	}

	const Graphics::Font& ResourceManager::getFont(ResourceHandle handle)
	{
		auto it = mResources.find(handle.ResourceIndex);
		AssertRet(it != mResources.end(), EMPTY_FONT, "Trying to access non loaded font");
		return static_cast<Graphics::Font&>(*(it->second.get()));
	}

	void ResourceManager::createResourceLock(const std::string& path)
	{
		mPathsMap[path] = mHandleIdx;
		mPathFindMap[mHandleIdx] = path;
		mResourceLocksCount[mHandleIdx] = 1;
	}

	void ResourceManager::loadOneAtlasData(const std::string &path)
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
			createResourceLock(path);
			mResources[mHandleIdx] = std::make_unique<Graphics::Texture>(new Internal::SdlSurface(path));
			return ResourceHandle(mHandleIdx++);
		}
	}

	ResourceHandle ResourceManager::lockSprite(const std::string& path)
	{
		auto it = mPathsMap.find(path);
		if (it != mPathsMap.end())
		{
			++mResourceLocksCount[it->second];
			return ResourceHandle(it->second);
		}
		else
		{
			createResourceLock(path);
			ResourceHandle originalTextureHandle;
			auto it = mAtlasFrames.find(path);
			if (it != mAtlasFrames.end())
			{
				originalTextureHandle = lockTexture(it->second.atlasPath);
				const Graphics::Texture& texture = getTexture(originalTextureHandle);
				mResources[mHandleIdx] = std::make_unique<Graphics::Sprite>(texture.getSurface(), it->second.quadUV);
			}
			else
			{
				originalTextureHandle = lockTexture(path);
				const Graphics::Texture& texture = getTexture(originalTextureHandle);
				mResources[mHandleIdx] = std::make_unique<Graphics::Sprite>(texture.getSurface(), Graphics::QuadUV());
			}

			mResourceReleaseFns[mHandleIdx] = [this, originalTextureHandle](Resource*){
				unlockResource(originalTextureHandle);
			};

			return ResourceHandle(mHandleIdx++);
		}
	}

	ResourceHandle ResourceManager::lockFont(const std::string& path)
	{
		auto it = mPathsMap.find(path);
		if (it != mPathsMap.end())
		{
			++mResourceLocksCount[it->second];
			return ResourceHandle(it->second);
		}
		else
		{
			createResourceLock(path);
			mResources[mHandleIdx] = std::make_unique<Graphics::Font>(new Internal::SdlSurface(path));
			return ResourceHandle(mHandleIdx++);
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
			createResourceLock(path);
			auto it = mAnimationData.find(path);
			AssertFatal(it != mAnimationData.end(), "No animation found with path %s", path.c_str());

			std::vector<ResourceHandle> frames;
			for (const auto& animFramePath : it->second.framePaths)
			{
				auto spriteHandle = lockSprite(animFramePath);
				frames.push_back(spriteHandle);
			}
			mResources[mHandleIdx] = std::make_unique<Graphics::SpriteAnimation>(std::move(frames));

			mResourceReleaseFns[mHandleIdx] = [this](Resource* resource){
				for (auto& spriteHandle : static_cast<Graphics::SpriteAnimation*>(resource)->getSprites())
				{
					unlockResource(spriteHandle);
				}
			};

			return ResourceHandle(mHandleIdx++);
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
			if (auto resourceIt = mResources.find(handle.ResourceIndex); resourceIt != mResources.end())
			{
				if (auto releaseFnIt = mResourceReleaseFns.find(handle.ResourceIndex); releaseFnIt != mResourceReleaseFns.end())
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
