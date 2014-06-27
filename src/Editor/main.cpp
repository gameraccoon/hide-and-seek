#include <Debug/Log.h>

#include <Engine/Core/World.h>
#include <Engine/Structures/PathPoint.h>
#include <Engine/Modules/LevelLoader.h>
#include <Engine/Modules/WorldsContainer.h>
#include <Engine/Modules/ActorFactory.h>
#include <Engine/Actors/LightEmitter.h>

#include <HgeInterface/Input/KeyListeners.h>
#include <HgeInterface/Graphics/FloatingCamera.h>

#include <Game/Actors/Hero.h>

#include <vector>

#include "actorsRegistration.h"

// Hge subsystem
HGE *hge = nullptr;

// Pointers to the HGE objects we will use
hgeSprite*	crosshair;
hgeFont*	font;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const bool FULL_SCREEN = false;

Vector2D mousePos(ZERO_VECTOR);
Vector2D worldMousePos(ZERO_VECTOR);


float cameraAngle = 0.0f;

// World that we edit
World *gameWorld = nullptr;

// Camera
Camera *mainCamera = nullptr;

Vector2D cameraWorldLocation(ZERO_VECTOR);

LightEmitter *cameraLight = nullptr;

IActor *actorToCreate = nullptr;

// Buttons
ButtonListeners listeners;


class BtnMouseL : public ButtonSwitcher
{
public:
	BtnMouseL(HGE *hge) : ButtonSwitcher(hge, HGEK_LBUTTON, true) { };
	void released()
	{
		::actorToCreate = nullptr;
	}
};

class BtnMouseR : public ButtonSwitcher
{
public:
	BtnMouseR(HGE *hge) : ButtonSwitcher(hge, HGEK_RBUTTON, true) { };
	void pressed()
	{
		::actorToCreate = ActorFactory::Factory().placeActor("Wall", gameWorld, worldMousePos, Vector2D(1.0f, 1.0f), 0.0f);
	}
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

	cameraWorldLocation += Direction;

	::hge->Input_GetMousePos(&::mousePos.x, &::mousePos.y);

	worldMousePos = mainCamera->deProject(mousePos);
	
	cameraLight->setLocation(worldMousePos);

	if (::actorToCreate != nullptr)
	{
		::actorToCreate->setLocation(worldMousePos);
	}

	::mainCamera->setLocation(cameraWorldLocation);
	::mainCamera->setRotation(::cameraAngle);

	// Update key states
	::listeners.check();

	return false;
}

bool RenderFunc()
{
	hgeSprite* cameraRenderSprite = new hgeSprite(::hge->Target_GetTexture(::mainCamera->getRenderTexture()), 0, 0,
		::mainCamera->getResolution().x, ::mainCamera->getResolution().y);

	::mainCamera->render();
	
	::hge->Gfx_BeginScene();
	::hge->Gfx_Clear(0);
	
	//-- Start render graphics on the screen

	cameraRenderSprite->Render(0, 0);

	::crosshair->Render(::mousePos.x, ::mousePos.y);

	// fps and dt
	::font->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d", ::hge->Timer_GetDelta(), ::hge->Timer_GetFPS());

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
	::hge->System_SetState(HGE_TITLE, "Stealth game - Editor");
	::hge->System_SetState(HGE_FPS, 100);
	::hge->System_SetState(HGE_WINDOWED, !FULL_SCREEN);
	::hge->System_SetState(HGE_SCREENWIDTH, SCREEN_WIDTH);
	::hge->System_SetState(HGE_SCREENHEIGHT, SCREEN_HEIGHT);
	::hge->System_SetState(HGE_SCREENBPP, 32);
	::hge->System_SetState(HGE_SHOWSPLASH, false); // hidding splash for develop-time only

	if(::hge->System_Initiate())
	{
		try
		{
			// Load sound and texture
			HTEXTURE texture = ::hge->Texture_Load("particles.png");

			// Load a font
			::font = new hgeFont("font1.fnt");
			::font->SetScale(0.7f);

			// Create and set up a particle system
			::crosshair = new hgeSprite(texture, 64, 96, 32, 32);
			::crosshair->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
			::crosshair->SetHotSpot(16, 16);

			FactoryActors::RegisterAll();

			::gameWorld = new World();
			WorldsContainer::Container().insertWorld(::gameWorld, "mainWorld");

			::mainCamera = new Camera(::hge, ::gameWorld, Vector2D((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT), Vector2D(0.0f, 0.0f));
			::mainCamera->showFog(false);

			cameraLight = new LightEmitter(::gameWorld, ZERO_VECTOR, Vector2D(0.0f, 0.0f), 0.0f);

			::listeners.addListener(new BtnMouseL(::hge));
			::listeners.addListener(new BtnMouseR(::hge));

			LevelLoader::load(::gameWorld, std::string("test"));

			// Let's rock now!
			::hge->System_Start();

			// Delete created objects and free loaded resources
			delete ::font;
			delete ::crosshair;
			delete ::mainCamera;
			delete ::gameWorld;
			::hge->Texture_Free(texture);
		}
		catch (std::runtime_error e)
		{
			Log::WriteError("Game load filed with unknown error!");
		}
	}
	else
	{
		MessageBox(NULL, "System failed to initialize", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		Log::WriteError("System failed to initialize");
	}

	// Clean up and shutdown
	::hge->System_Shutdown();
	::hge->Release();
	return 0;
}
