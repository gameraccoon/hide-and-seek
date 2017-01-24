#include "Base/ResourceManager.h"

#include <fstream>
#include <vector>

#include <Log.h>

#include "Base/Engine.h"
#include "../Internal/SdlSurface.h"


class ResourceManager::Impl : public IUseCounter
{
public:
	std::vector<std::unique_ptr<Resource::Base>> resources;
	std::map<std::string, IUseCounter::Uid> texturesMap;
	std::map<std::string, IUseCounter::Uid> fontsMap;

	Engine::Internal::Engine* engine;

	virtual void beginUse(Uid uid) override;
	virtual void endUse(Uid uid) override;
	void releaseResource(Uid uid);
};

void ResourceManager::Impl::beginUse(Uid uid)
{
	if (uid < 0 || uid >= resources.size())
	{
		Log::Instance().writeError("Inadequate resource UID");
		return;
	}

	auto& resourcePtr = resources[uid];
	if (resourcePtr)
	{
		resourcePtr->useCount++;
	}
	else
	{
		Log::Instance().writeError("Trying to access released resource");
	}
}

void ResourceManager::Impl::endUse(Uid uid)
{
	if (uid < 0 || uid >= resources.size())
	{
		Log::Instance().writeError("Inadequate resource UID");
		return;
	}

	auto& resourcePtr = resources[uid];
	if (resourcePtr)
	{
		resourcePtr->useCount--;

		if (resourcePtr->useCount <= 0)
		{
			releaseResource(uid);
		}
	}
	else
	{
		Log::Instance().writeError("Trying to access released resource");
	}
}

void ResourceManager::Impl::releaseResource(Uid uid)
{
	if (uid < 0 || uid >= resources.size())
	{
		Log::Instance().writeError("Inadequate resource UID");
		return;
	}

	// just release the resource but do not delete other related info
	resources[uid].reset();
}


ResourceManager::ResourceManager(Engine::Internal::Engine* engine)
	: mPimpl(new Impl())
{
	mPimpl->engine = engine;
}

ResourceManager::~ResourceManager()
{
	Log::Instance().writeLog("ResourceManager destroyed");
}

template <typename T, typename Base>
T getResource(const std::string& texturePath,
	std::map<std::string, IUseCounter::Uid>& nameMap,
	std::vector<std::unique_ptr<Resource::Base>>& resources,
	IUseCounter* useCounter,
	std::function<void(Base*)> fillResource,
	std::function<void(Resource::Base*)> destroyResource
)
{
	std::map<std::string, IUseCounter::Uid>::const_iterator it = nameMap.find(texturePath);

	if (it == nameMap.cend())
	{
		// there are no resource and info about it
		IUseCounter::Uid uid = (IUseCounter::Uid)resources.size();
		nameMap.insert({ texturePath, uid });
		Base* unsafePointer = new Base(uid, destroyResource);
		fillResource(unsafePointer);
		resources.emplace_back(unsafePointer);
		return T(useCounter, unsafePointer);
	}
	else
	{
		IUseCounter::Uid uid = it->second;

		if (uid < 0 || uid >= resources.size())
		{
			Log::Instance().writeError("Inadequate resource UID");
			return T(useCounter, nullptr);
		}

		Base* unsafePointer = dynamic_cast<T::Base*>(resources[uid].get());
		if (unsafePointer)
		{
			return T(useCounter, unsafePointer);
		}
		else
		{
			unsafePointer = new Base(uid, destroyResource);
			fillResource(unsafePointer);
			resources[uid].reset(unsafePointer);
			return T(useCounter, unsafePointer);
		}
	}
}

Texture ResourceManager::getTexture(const std::string& texturePath)
{
	Engine::Internal::Engine* engine = mPimpl->engine;

	return getResource<Texture, Texture::Base>(texturePath,
		mPimpl->texturesMap,
		mPimpl->resources,
		mPimpl.get(),
		[&texturePath, engine](Texture::Base* textureBase)
		{
			textureBase->surface = new Engine::Internal::SdlSurface(texturePath.c_str());
			textureBase->engine = engine;
		},
		[](Resource::Base* resourceBase)
		{
			Texture::Base* textureBase = dynamic_cast<Texture::Base*>(resourceBase);
			if (textureBase)
			{
				delete textureBase->surface;
				textureBase->surface = nullptr;
			}
			else
			{
				Log::Instance().writeError("Trying to destruct non-texture resource with texture destructer");
			}
		}
	);
}

Font ResourceManager::getFont(const std::string & texturePath)
{
	Engine::Internal::Engine* engine = mPimpl->engine;

	return getResource<Font, Font::Base>(texturePath,
		mPimpl->texturesMap,
		mPimpl->resources,
		mPimpl.get(),
		[&texturePath, engine](Font::Base* textureBase)
		{
			textureBase->surface = new Engine::Internal::SdlSurface(texturePath.c_str());
			textureBase->engine = engine;
		},
		[](Resource::Base* resourceBase)
		{
			Font::Base* textureBase = dynamic_cast<Font::Base*>(resourceBase);
			if (textureBase)
			{
				delete textureBase->surface;
				textureBase->surface = nullptr;
			}
			else
			{
				Log::Instance().writeError("Trying to destruct non-texture resource with texture destructer");
			}
		}
	);
}

/*
engineSprite* ResourceManager::getSprite(std::string spriteId)
{
	if (mEngine == nullptr)
	{
		std::string error = "GraphicLoader isn't initialised";
		Log::Instance().writeError(error);
		throw new std::runtime_error(error);
	}

	SpriteMap::iterator it = mSprites.find(spriteId);
	if (it != mSprites.end())
	{ // if we have this sprite in map
		return it->second;
	}
	else
	{
		// get sprite info from outer source
		SpriteInfo spriteInfo = loadSpriteInfo(spriteId);
		// get existed texture or load from file
		engineSprite* sprite = nullptr;
		
		if (spriteInfo.loaded)
		{
			HTEXTURE texture = getTexture(spriteInfo.textureName);
			sprite = new engineSprite(texture, (float)spriteInfo.posX, (float)spriteInfo.posY,
				(float)spriteInfo.sizeX, (float)spriteInfo.sizeY);
			sprite->SetColor(spriteInfo.color);
			sprite->SetHotSpot((float)spriteInfo.hotSpotX, (float)spriteInfo.hotSpotY);
		}

		mSprites.insert(SpriteMap::value_type(spriteId, sprite));

		return sprite;
	}
}*/

ResourceManager::SpriteInfo ResourceManager::loadSpriteInfo(const std::string& id)
{
	SpriteInfo spriteInfo;
	spriteInfo.loaded = false;

	std::ifstream graphicInfoFile;
	try
	{
		graphicInfoFile.open(mGraphicInfoFileName);
	}
	catch(std::exception)
	{
		return spriteInfo;
	}

	while (!graphicInfoFile.eof())
	{
		std::string className;
		graphicInfoFile >> className; // get from filestream
		
		if (className.compare(std::string("[").append(id).append("]")) != 0)
			continue;

		// if we got required sprite
		graphicInfoFile >> spriteInfo.textureName;
		graphicInfoFile >> spriteInfo.posX;
		graphicInfoFile >> spriteInfo.posY;
		graphicInfoFile >> spriteInfo.sizeX;
		graphicInfoFile >> spriteInfo.sizeY;
		graphicInfoFile >> spriteInfo.hotSpotX;
		graphicInfoFile >> spriteInfo.hotSpotY;
		//graphicInfoFile >> spriteInfo.color;

		std::string endDelimeter;
		graphicInfoFile >> endDelimeter;

		spriteInfo.loaded = endDelimeter.compare("end") == 0;

		break;
	}
	graphicInfoFile.close();

	return spriteInfo;
}
