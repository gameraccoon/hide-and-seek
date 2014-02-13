#include "../src/Globals.h"
#include "../src/Hero.h"
#include "../src/DirectionArrow.h"
#include "../src/World.h"
#include "../src/Wall.h"
#include "../src/FloatingCamera.h"
#include "../src/KeyListeners.h"
#include "../src/LightEmitter.h"
#include "../src/PathPoint.h"
#include "../src/LevelLoader.h"
#include <vector>

// Hge subsystem
HGE *hge = NULL;

// Pointers to the HGE objects we will use
hgeSprite*	crosshair;
hgeFont*	font;

//#define FULLSCREEN
#ifndef FULLSCREEN
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const bool FULL_SCREEN = false;
#else
	const int SCREEN_WIDTH = 1366;
	const int SCREEN_HEIGHT = 768;
	const bool FULL_SCREEN = true;
#endif

const Vector2D SCREEN_CENTER(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

const float MAX_CAMERA_RANGE = std::min(SCREEN_HEIGHT, SCREEN_WIDTH) / 4.0f;

Vector2D mousePos = ZERO_VECTOR;

float cameraAngle = 0.0f;

// The Hero whom we control
Hero *ourHero;

// Our big World =)
World *gameWorld;

// Camera
FloatingCamera *mainCamera;

// Test arrow for show directions on screen
DirectionArrow *arrow;

// Button
ButtonListeners listeners;

// event listeners
class BtnShadows : public ButtonSwitcher
{
public:
	BtnShadows(HGE *hge) : ButtonSwitcher(hge, HGEK_H, true) { };
	void pressed() { ::mainCamera->showShadows(bActive); }
};

class BtnFog : public ButtonSwitcher
{
public:
	BtnFog(HGE *hge) : ButtonSwitcher(hge, HGEK_F, true) { };
	void pressed() { ::mainCamera->showFog(bActive); }
};

class BtnLights : public ButtonSwitcher
{
public:
	BtnLights(HGE *hge) : ButtonSwitcher(hge, HGEK_L, false) { };
	void pressed() { ::mainCamera->showLights(bActive); }
};

class BtnAABB : public ButtonSwitcher
{
public:
	BtnAABB(HGE *hge) : ButtonSwitcher(hge, HGEK_C, false) { };
	void pressed() { ::mainCamera->showAABB(bActive); }
};

class BtnHulls : public ButtonSwitcher
{
public:
	BtnHulls(HGE *hge) : ButtonSwitcher(hge, HGEK_M, false) { };
	void pressed() { ::mainCamera->showHulls(bActive); }
};

class BtnPaths : public ButtonSwitcher
{
public:
	BtnPaths(HGE *hge) : ButtonSwitcher(hge, HGEK_P, false) { };
	void pressed() { ::mainCamera->showPaths(bActive); }
};

class BtnShoot : public ButtonSwitcher
{
public:
	BtnShoot(HGE *hge) : ButtonSwitcher(hge, HGEK_LBUTTON, true) { };
	void pressed() { ::ourHero->startShoting(::mainCamera->deProject(::mousePos)); }
	void released() { ::ourHero->stopShoting(); }
};

class BtnAddLight : public ButtonSwitcher
{
public:
	BtnAddLight(HGE *hge) : ButtonSwitcher(hge, HGEK_RBUTTON, true) { };
	void pressed() { new LightEmitter(::gameWorld, ::mainCamera->deProject(::mousePos)); }
};

bool FrameFunc()
{
	float dt = ::hge->Timer_GetDelta();

	// Process keys
	if (::hge->Input_GetKeyState(HGEK_ESCAPE)) return true;

	Vector2D Direction = ZERO_VECTOR;
	if (::hge->Input_GetKeyState(HGEK_A))	Direction += LEFT_DIRECTION;
	if (::hge->Input_GetKeyState(HGEK_D))	Direction += RIGHT_DIRECTION;
	if (::hge->Input_GetKeyState(HGEK_W))	Direction += UP_DIRECTION;
	if (::hge->Input_GetKeyState(HGEK_S))	Direction += DOWN_DIRECTION;

	if (::hge->Input_GetKeyState(HGEK_Q))	::cameraAngle += 0.005f;
	if (::hge->Input_GetKeyState(HGEK_E))	::cameraAngle -= 0.005f;
	

	::ourHero->move(Vector2D(Direction.rotation() - ::cameraAngle) * Direction.ort().size() * 100); // constant speed
	::ourHero->setRotation((::mainCamera->deProject(::mousePos) - ::ourHero->getLocation()).rotation());

	::hge->Input_GetMousePos(&::mousePos.x, &::mousePos.y);

	Vector2D CameraShift((::mousePos - SCREEN_CENTER)/2);
	CameraShift = CameraShift.ort() * std::min(CameraShift.size(), MAX_CAMERA_RANGE);
	::mainCamera->setLocation(::ourHero->getLocation());
	::mainCamera->setRotation(::cameraAngle);
	::mainCamera->setCenterShift(CameraShift);

	// Update key states
	::listeners.check();

	// Do World update
	::gameWorld->update(dt);

	::arrow->setVDirection(Direction);
	::arrow->setCenter(SCREEN_CENTER - CameraShift);

	return false;
}

bool RenderFunc()
{
	hgeSprite* cameraRenderSprite = new hgeSprite(::hge->Target_GetTexture(::mainCamera->getRenderTexture()), 0, 0,
		::mainCamera->getResolution().x, ::mainCamera->getResolution().y);

	::mainCamera->render();
	
	::hge->Gfx_BeginScene();
	::hge->Gfx_Clear(0);
	
	//-- Start renders graphics

	cameraRenderSprite->Render(0, 0);

	::crosshair->Render(::mousePos.x, ::mousePos.y);
	::arrow->render();

	// fps and dt
	::font->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d", ::hge->Timer_GetDelta(), ::hge->Timer_GetFPS());
	
	// Status of rendering elements
	if (::listeners.isActive(HGEK_M))
		::font->printf(5, 60, HGETEXT_LEFT, "Showing Models");
	if (::listeners.isActive(HGEK_C))
		::font->printf(5, 90, HGETEXT_LEFT, "Showing Bounding boxes");
	if (::listeners.isActive(HGEK_L))
		::font->printf(5, 120, HGETEXT_LEFT, "Showing Light emitters");
	if (::listeners.isActive(HGEK_P))
		::font->printf(5, 150, HGETEXT_LEFT, "Showing Paths");

	if (!::listeners.isActive(HGEK_F))
		::font->printf(5, 180, HGETEXT_LEFT, "Hidded Fog");
	if (!::listeners.isActive(HGEK_H))
		::font->printf(5, 210, HGETEXT_LEFT, "Hidded Shadows");

	//-- Stop render graphics

	::hge->Gfx_EndScene();

	delete cameraRenderSprite;

	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	::hge = hgeCreate(HGE_VERSION);

	::hge->System_SetState(HGE_LOGFILE, "SG.log");
	::hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	::hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	::hge->System_SetState(HGE_TITLE, "Stealth game - alpha1");
	::hge->System_SetState(HGE_FPS, 100);
	::hge->System_SetState(HGE_WINDOWED, !FULL_SCREEN);
	::hge->System_SetState(HGE_SCREENWIDTH, SCREEN_WIDTH);
	::hge->System_SetState(HGE_SCREENHEIGHT, SCREEN_HEIGHT);
	::hge->System_SetState(HGE_SCREENBPP, 32);
	::hge->System_SetState(HGE_SHOWSPLASH, false); // hidding splash for develop-time only

	if(::hge->System_Initiate())
	{
		// Load sound and texture
		HTEXTURE texture = ::hge->Texture_Load("particles.png");
		if(!texture)
		{
			// If one of the data files is not found, display
			// an error message and shutdown.
			MessageBox(NULL, "Can't load one of the following files:\nFONT1.FNT, FONT1.PNG, PARTICLES.PNG", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			::hge->System_Shutdown();
			::hge->Release();
			return 0;
		}

		// Load a font
		::font = new hgeFont("font1.fnt");
		::font->SetScale(0.7f);

		// Create and set up a particle system
		::crosshair = new hgeSprite(texture, 64, 96, 32, 32);
		::crosshair->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		::crosshair->SetHotSpot(16, 16);

		::gameWorld = new World(::hge);

		::mainCamera = new FloatingCamera(::gameWorld, SCREEN_CENTER * 2, Vector2D(0.0f, 0.0f));

		// hero will be deleted automaticaly as other actors
		::ourHero = new Hero(::gameWorld, Vector2D(0.0f, 350.0f));

		::ourHero->giveWeapon(new Weapon());

		::arrow = new DirectionArrow(::hge);
		::arrow->setCenter(SCREEN_CENTER);
		
		::listeners.addListener(new BtnAABB(::hge));
		::listeners.addListener(new BtnHulls(::hge));
		::listeners.addListener(new BtnFog(::hge));
		::listeners.addListener(new BtnShadows(::hge));
		::listeners.addListener(new BtnLights(::hge));
		::listeners.addListener(new BtnPaths(::hge));
		::listeners.addListener(new BtnShoot(::hge));
		::listeners.addListener(new BtnAddLight(::hge));

		LevelLoader::load(::gameWorld, std::string("test"));
		//LevelLoader::save(GameWorld, std::string("test"));

		// Let's rock now!
		::hge->System_Start();

		// Delete created objects and free loaded resources
		delete ::font;
		delete ::crosshair;
		delete ::arrow;
		delete ::mainCamera;
		delete ::gameWorld;
		::hge->Texture_Free(texture);
	}
	else
	{
		MessageBox(NULL, "System failed to initialize", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}

	// Clean up and shutdown
	::hge->System_Shutdown();
	::hge->Release();
	return 0;
}
