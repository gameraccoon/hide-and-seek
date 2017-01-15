#include "ResourceManager.h"

#include <fstream>
#include <vector>

#include <Debug/Log.h>

#include <EngineInterface/Internal/SdlSurface.h>
#include <EngineInterface/Internal/Engine.h>


class ResourceManager::Impl : public IUseCounter
{
public:
	std::vector<std::unique_ptr<Resource::Base>> resources;
	std::map<std::string, IUseCounter::Uid> texturesMap;

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

static void FillTextureBase(Texture::Base* textureBase, const std::string& path, Engine::Internal::Engine* engine)
{
	textureBase->surface = new Engine::Internal::SdlSurface(path.c_str());
	textureBase->engine = engine;
}

static void TextureDestructor(Resource::Base* resourceBase)
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

Texture::Ptr ResourceManager::getTexture(const std::string& texturePath)
{
	std::map<std::string, IUseCounter::Uid>::const_iterator it = mPimpl->texturesMap.find(texturePath);

	if (it == mPimpl->texturesMap.cend())
	{
		// there are no texture and info about it
		IUseCounter::Uid uid = mPimpl->resources.size();
		mPimpl->texturesMap.insert({ texturePath, uid });
		Texture::Base* unsafePointer = new Texture::Base(uid, &TextureDestructor);
		FillTextureBase(unsafePointer, texturePath, mPimpl->engine);
		mPimpl->resources.emplace_back(unsafePointer);
		return std::make_unique<Texture>(mPimpl.get(), unsafePointer);
	}
	else
	{
		IUseCounter::Uid uid = it->second;

		if (uid < 0 || uid >= mPimpl->resources.size())
		{
			Log::Instance().writeError("Inadequate resource UID");
			return nullptr;
		}

		Texture::Base* unsafePointer = dynamic_cast<Texture::Base*>(mPimpl->resources[uid].get());
		if (unsafePointer)
		{
			return std::make_unique<Texture>(mPimpl.get(), unsafePointer);
		}
		else
		{
			Log::Instance().writeError("Try to access empty texture");
			return nullptr;
		}
	}
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
