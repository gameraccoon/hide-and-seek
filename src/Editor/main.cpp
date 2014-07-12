#include <Debug/Log.h>

#include <Engine/Core/World.h>
#include <Engine/Structures/PathPoint.h>
#include <Engine/Modules/LevelLoader.h>
#include <Engine/Modules/WorldsContainer.h>
#include <Engine/Modules/ActorFactory.h>
#include <Engine/Actors/LightEmitter.h>

#include <HgeInterface/Input/KeyListeners.h>
#include <HgeInterface/Graphics/FloatingCamera.h>

#include <StealthGame/Actors/Hero.h>

#include <vector>

#include "actorsRegistration.h"

#include "ControlElements/TransformationShell.h"

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

Vector2D selectedActorDragPoint(ZERO_VECTOR);
IActor *selectedActor = nullptr;

TransformationShell *transformShell;
bool isDraggingActor = false;

// Buttons
ButtonListeners listeners;

std::vector<std::string> actorsToSpawn;
int currentSpawnActorIndex = 0;

bool bViewHolded = false;
Vector2D lastCameraPos(ZERO_VECTOR);
Vector2D lastMousePos(ZERO_VECTOR);

class BtnMouseL : public ButtonSwitcher
{
public:
	BtnMouseL(HGE *hge) : ButtonSwitcher(hge, HGEK_LBUTTON, true) { };
	void pressed()
	{
		for (auto actor : ::gameWorld->allActors)
		{
			Vector2D location = actor->getLocation();
			BoundingBox aabb = actor->getBoundingBox();
			if (this->getDotCode(aabb, ::mainCamera->deProject(::mousePos)) == 0)
			{
				::selectedActor = actor;
				::selectedActorDragPoint = mainCamera->deProject(::mousePos) - actor->getLocation();
				isDraggingActor = true;
				break;
			}
		}

		if (!isDraggingActor)
		{
			if (::hge->Input_GetKeyState(HGEK_CTRL))
			{
				::selectedActor = ActorFactory::Factory().placeActor(actorsToSpawn[currentSpawnActorIndex],
					gameWorld, mainCamera->deProject(::mousePos), Vector2D(1.0f, 1.0f), 0.0f);
			}
		}
	}
	void released()
	{
		isDraggingActor = false;
	}
private:
	static const int LEFT_BIT = 1;
	static const int RIGHT_BIT = 2;
	static const int BOTTOM_BIT = 4;
	static const int TOP_BIT = 8;

	inline int getDotCode(const BoundingBox &box, const Vector2D &dot)
	{
		return (((dot.x < box.minX) ? LEFT_BIT : 0)
				| ((dot.x > box.maxX) ? RIGHT_BIT : 0)
				| ((dot.y < box.minY) ? BOTTOM_BIT : 0)
				| ((dot.y > box.maxY) ? TOP_BIT : 0));
	}
};

class BtnMouseR : public ButtonSwitcher
{
public:
	BtnMouseR(HGE *hge) : ButtonSwitcher(hge, HGEK_RBUTTON, true) { };

	void pressed()
	{
		::bViewHolded = true;
		::lastCameraPos = ::cameraWorldLocation;
		::lastMousePos = ::mousePos;
	}

	void released()
	{
		::bViewHolded = false;
	}
};

class BtnSave : public ButtonSwitcher
{
public:
	BtnSave(HGE *hge) : ButtonSwitcher(hge, HGEK_S, true) { };

	void pressed()
	{
		if (::hge->Input_GetKeyState(HGEK_CTRL))
		{
			LevelLoader::save(::gameWorld, std::string("test"));
		}
	}
};

class BtnCancelPlacing : public ButtonSwitcher
{
public:
	BtnCancelPlacing(HGE *hge) : ButtonSwitcher(hge, HGEK_DELETE, true) { };

	void pressed()
	{
		if (::selectedActor != nullptr)
		{
			::selectedActor->destroy();
			::selectedActor = nullptr;
			::isDraggingActor = false;
		}
	}
};

bool FrameFunc()
{
	float dt = ::hge->Timer_GetDelta();

	// Process keys
	if (::hge->Input_GetKeyState(HGEK_ESCAPE)) return true;

	Vector2D Direction = ZERO_VECTOR;
	if (::hge->Input_GetKeyState(HGEK_LEFT))	Direction += LEFT_DIRECTION;
	if (::hge->Input_GetKeyState(HGEK_RIGHT))	Direction += RIGHT_DIRECTION;
	if (::hge->Input_GetKeyState(HGEK_UP))	Direction += UP_DIRECTION;
	if (::hge->Input_GetKeyState(HGEK_DOWN))	Direction += DOWN_DIRECTION;

	if (::hge->Input_GetKeyState(HGEK_Q))	::cameraAngle += 0.005f;
	if (::hge->Input_GetKeyState(HGEK_E))	::cameraAngle -= 0.005f;

	cameraWorldLocation += Direction;

	::hge->Input_GetMousePos(&::mousePos.x, &::mousePos.y);

	if (isDraggingActor && ::selectedActor != nullptr)
	{
		worldMousePos = mainCamera->deProject(::mousePos);
		::selectedActor->setLocation(worldMousePos - ::selectedActorDragPoint);
	}

	if (selectedActor != nullptr)
	{
		::transformShell->setScreenLocation(mainCamera->project(selectedActor->getLocation()));
	}

	if (::bViewHolded)
	{
		::cameraWorldLocation = ::lastCameraPos - ::mousePos + ::lastMousePos;
	}

	::mainCamera->setLocation(::cameraWorldLocation);
	::mainCamera->setRotation(::cameraAngle);

	hgeInputEvent * event = new hgeInputEvent();
	if (::hge->Input_GetEvent(event) && event->wheel != 0)
	{
		::currentSpawnActorIndex += event->wheel > 0 ? 1 : -1;

		if (::currentSpawnActorIndex < 0)
		{
			::currentSpawnActorIndex = ::actorsToSpawn.size() - 1;
		}

		if (::currentSpawnActorIndex >= (int)::actorsToSpawn.size())
		{
			::currentSpawnActorIndex = 0;
		}
	}
	delete event;

	// Update key states
	::listeners.check();

	::gameWorld->update(dt);

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
	
	// Object ready to spawn
	::font->printf(5, 60, HGETEXT_LEFT, ::actorsToSpawn[currentSpawnActorIndex].c_str());

	if (::selectedActor != nullptr)
	{
		::transformShell->render();
	}

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

#if (defined DEBUG) && (!defined RELEASE)
	 // hidding HGE splash for develop-time only
	::hge->System_SetState(HGE_SHOWSPLASH, false);
#endif

	if(::hge->System_Initiate())
	{
		try
		{
			GraphicLoader::Instance().init(hge, "./configs/textures.conf", "./");

			// Load a font
			::font = new hgeFont("font1.fnt");
			::font->SetScale(0.7f);

			// Create and set up a particle system
			::crosshair = GraphicLoader::Instance().getSprite("particles");
			::crosshair->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);

			::transformShell = new TransformationShell(::hge);

			FactoryActors::RegisterAll();

			::gameWorld = new World();
			WorldsContainer::Container().insertWorld(::gameWorld, "mainWorld");

			::mainCamera = new Camera(::hge, ::gameWorld, Vector2D((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT), Vector2D(0.0f, 0.0f));
			::mainCamera->showFog(false);
			::mainCamera->showLights(true);

			::listeners.addListener(new BtnMouseL(::hge));
			::listeners.addListener(new BtnMouseR(::hge));
			::listeners.addListener(new BtnSave(::hge));
			::listeners.addListener(new BtnCancelPlacing(::hge));
			
			::actorsToSpawn.push_back("Wall");
			::actorsToSpawn.push_back("LightEmitter");
			::actorsToSpawn.push_back("Man");

			LevelLoader::load(::gameWorld, std::string("test"));

			// Let's rock now!
			::hge->System_Start();

			// Delete created objects and free loaded resources
			delete ::font;
			delete ::mainCamera;
			delete ::gameWorld;
		}
		catch (std::runtime_error e)
		{
			Log::Instance().writeError(std::string("Game load filed with error: ").append(+ e.what()));
		}
	}
	else
	{
		MessageBox(NULL, "System failed to initialize", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		Log::Instance().writeError("System failed to initialize");
	}

	// Clean up and shutdown
	::hge->System_Shutdown();
	::hge->Release();
	return 0;
}
