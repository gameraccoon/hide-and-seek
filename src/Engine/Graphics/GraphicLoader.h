#ifndef GRAPHIC_LOADER
#define GRAPHIC_LOADER

#include <string>
#include <map>

#include "../Hge/HgeIncludes.h"


/**
 * Exception that throws when path to texture was incorrect.
 */
class TextureNotFoundException : public std::exception { };

/**
 * Class able to read Textures and create and sprites.
 *
 * If texture load once that it unload only after destruction.
 */
class GraphicLoader
{
public:
	GraphicLoader(HGE* hge, std::string graphicInfoFile, std::string imgFolder);
	~GraphicLoader(void);
	/**
	 * @throws TextureNotFoundException
	 */
	hgeSprite* getSprite(std::string classId);

private:
	// types
	struct SpriteInfo
	{
		std::string id;
		std::string textureName;
		int sizeX, sizeY;
		DWORD color;
		int hotSpotX, hotSpotY;
		bool loaded;
		byte asd;
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
	/**
	 * @throws TextureNotFoundException
	 */
	HTEXTURE getTexture(std::string name);
};

#endif