#ifndef GRAPHIC_LOADER
#define GRAPHIC_LOADER

#include <string>
#include <map>

#include <hge.h>
#include <hgesprite.h>
#include <hgefont.h>
#include <hgeresource.h>


/**
 * Exception that throws when path to texture was incorrect.
 */
class TextureNotFoundException : public std::exception { };

/**
 * Class able to read Textures and create and sprites.
 *
 * If texture load once that it unload only after destruction.
 *
 * GraphicLoader is a singleton.
 */
class GraphicLoader
{
public:
	/**
	 * Return single instance of class Log
	 * Usage: GraphicLoader::Instance().function(...);
	 */
	static GraphicLoader& Instance();

	void init(HGE* hge, std::string graphicInfoFile, std::string imgFolder);

	/**
	 * @throws TextureNotFoundException
	 */
	hgeSprite* getSprite(std::string spriteId);
	
	/**
	 * @throws TextureNotFoundException
	 */
	HTEXTURE getTexture(std::string name);
private:
	// types
	struct SpriteInfo
	{
		std::string id;
		std::string textureName;
		int posX, posY;
		int sizeX, sizeY;
		DWORD color;
		int hotSpotX, hotSpotY;
		bool loaded;
	};

	typedef std::map<std::string, HTEXTURE> TextureMap;
	typedef std::map<std::string, hgeSprite*> SpriteMap;

	// fields
	TextureMap textures;
	SpriteMap sprites;

	HGE* hge;

	std::string graphicInfoFileName;
	std::string imageFolder;

	// methods
	SpriteInfo loadSpriteInfo(std::string id);

	static GraphicLoader* singleInstance;

	/*
	 * Turn off unusable operations
	 */
	GraphicLoader();
	~GraphicLoader();
	GraphicLoader(const GraphicLoader&);
	void operator=(const GraphicLoader&);
};

#endif