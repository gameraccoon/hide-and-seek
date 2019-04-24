#include "Base/ResourceManager.h"

#include <fstream>
#include <vector>

#include <Debug/Log.h>
#include <Debug/Assert.h>

#include "Base/Engine.h"
#include "../Internal/SdlSurface.h"

namespace SystemInterface
{
	static Graphics::Texture EMPTY_TEXTURE = Graphics::Texture(nullptr);
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

	ResourceHandle ResourceManager::lockTexture(const std::string& path)
	{
		auto it = mPathsMap.find(path);
		if (it != mPathsMap.end())
		{
			++mResourceLocksCount[it->second];
			return it->second;
		}
		else
		{
			createResourceLock(path);
			mResources[mHandleIdx] = std::make_unique<Graphics::Texture>(new Internal::SdlSurface(path.c_str()));
			return mHandleIdx++;
		}
	}

	ResourceHandle ResourceManager::lockFont(const std::string& path)
	{
		auto it = mPathsMap.find(path);
		if (it != mPathsMap.end())
		{
			++mResourceLocksCount[it->second];
			return it->second;
		}
		else
		{
			createResourceLock(path);
			mResources[mHandleIdx] = std::make_unique<Graphics::Font>(new Internal::SdlSurface(path.c_str()));
			return mHandleIdx++;
		}
	}

	void ResourceManager::unlockResource(ResourceHandle handle)
	{
		auto it = mResourceLocksCount.find(handle.ResourceIndex);
		AssertRetVoid(it != mResourceLocksCount.end(), "Unlocking non-locked resource");
		if (it->second > 1)
		{
			--it->second;
			return;
		}
		else
		{
			// unload resource
			mResources.erase(handle.ResourceIndex);
			mResourceLocksCount.erase(handle.ResourceIndex);
			auto pathIt = mPathFindMap.find(handle.ResourceIndex);
			if (pathIt != mPathFindMap.end())
			{
				mPathsMap.erase(pathIt->second);
			}
			mPathFindMap.erase(handle.ResourceIndex);
		}
	}
}
