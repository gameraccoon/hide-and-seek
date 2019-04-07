#include <Debug/Log.h>

#include <Engine/Core/World.h>
#include <Engine/Structures/PathPoint.h>
#include <Engine/Modules/LevelLoader.h>
#include <Engine/Modules/WorldsContainer.h>
#include <Engine/Modules/ActorFactory.h>
#include <Engine/Actors/LightEmitter.h>

#include <EngineInterface/Input/KeyListeners.h>
#include <EngineInterface/Graphics/FloatingCamera.h>

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


float cameraAngle = 0.0f;

// World that we edit
World *gameWorld = nullptr;

// Camera
Camera *mainCamera = nullptr;

Vector2D cameraWorldLocation(ZERO_VECTOR);

IActor *selectedActor = nullptr;
Vector2D selectedActorDragPoint(ZERO_VECTOR);
Vector2D selectedActorInitialScale(ZERO_VECTOR);
Rotator selectedActorInitialRotation(0.0f);

TransformationShell *transformShell = nullptr;

// Buttons
ButtonListeners listeners;

std::vector<std::string> actorsToSpawn;
int currentSpawnActorIndex = 0;

bool bViewHolded = false;
Vector2D lastCameraPos(ZERO_VECTOR);
Vector2D lastMousePos(ZERO_VECTOR);

enum class TransformationType
{
	Move
	,Rotate
	,Scale
	,None
};

TransformationType transform = TransformationType::None;

void SelectActor(IActor *actor)
{
	::selectedActor = actor;
	
	if (::transformShell != nullptr)
	{
		delete ::transformShell; ::transformShell = nullptr;
	}
				
	::transformShell = new TransformationShell(::hge, actor);
}

void UnSelect()
{
	::selectedActor = nullptr;
	
	delete ::transformShell; ::transformShell = nullptr;
}

class BtnMouseL : public ButtonSwitcher
{
public:
	BtnMouseL(HGE *hge) : ButtonSwitcher(hge, HGEK_LBUTTON, true) { };
	void pressed()
	{
		bool processed = false;

		if (::transformShell != nullptr)
		{
			TransformationShell::ModificationEvent trEvent = ::transformShell->checkButton(mousePos);
			
			switch (trEvent)
			{
			case TransformationShell::Move:
				::selectedActorDragPoint = mainCamera->deProject(::mousePos) - ::selectedActor->getLocation();
				transform = TransformationType::Move;
				processed = true;
				break;
			case TransformationShell::Scale:
				::selectedActorInitialScale = ::selectedActor->getScale();
				::selectedActorDragPoint = mainCamera->deProject(::mousePos);
				transform = TransformationType::Scale;
				processed = true;
				break;
			case TransformationShell::Rotate:
				::selectedActorInitialRotation = ::selectedActor->getRotation();
				::selectedActorDragPoint = mainCamera->deProject(::mousePos);
				transform = TransformationType::Rotate;
				processed = true;
				break;
			default:
				transform = TransformationType::None;
				break;
			}
		}
		
		if (!processed)
		{
			for (auto actor : ::gameWorld->allActors)
			{
				Vector2D location = actor->getLocation();
				BoundingBox aabb = actor->getBoundingBox();
				if (actor != ::selectedActor && this->getDotCode(aabb, ::mainCamera->deProject(::mousePos)) == 0)
				{
					::SelectActor(actor);
					processed = true;
					break;
				}
			}
		}

		if (!processed && ::hge->Input_GetKeyState(HGEK_CTRL))
		{
			
			::SelectActor(ActorFactory::Factory().placeActor(actorsToSpawn[currentSpawnActorIndex],
				gameWorld, mainCamera->deProject(::mousePos), Vector2D(1.0f, 1.0f), 0.0f));
			processed = true;
		}

		if (!processed)
		{
			::UnSelect();
		}
	}

	void released()
	{
		transform = TransformationType::None;
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
			::UnSelect();
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

	switch (transform)
	{
	case TransformationType::Move:
		{
			Vector2D worldMousePos = mainCamera->deProject(::mousePos);
			::selectedActor->setLocation(worldMousePos - ::selectedActorDragPoint);
			break;
		}
	case TransformationType::Scale:
		{
			Vector2D worldMousePos = mainCamera->deProject(::mousePos);
			Vector2D absScale(worldMousePos - ::selectedActor->getLocation());
			Vector2D oldScale(::selectedActorDragPoint - ::selectedActor->getLocation());

			// abs
			absScale.x = absScale.x >= 0 ? absScale.x : -absScale.x;
			absScale.y = absScale.y >= 0 ? absScale.y : -absScale.y;
			oldScale.x = oldScale.x >= 0 ? oldScale.x : -oldScale.x;
			oldScale.y = oldScale.y >= 0 ? oldScale.y : -oldScale.y;

			::selectedActor->setScale((absScale - oldScale)/30.0 + ::selectedActorInitialScale);
			break;
		}
	case TransformationType::Rotate:
		{
			Vector2D worldMousePos = mainCamera->deProject(::mousePos);
			Rotator absRotate((worldMousePos - ::selectedActor->getLocation()).rotation());
			Rotator oldRotate((::selectedActorDragPoint - ::selectedActor->getLocation()).rotation());
			::selectedActor->setRotation((absRotate - oldRotate) + ::selectedActorInitialRotation);
			break;
		}
	default:
		break;
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

	if (::transformShell != nullptr)
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
			LogError(std::string("Game load filed with error: ").append(+ e.what()));
		}
	}
	else
	{
		MessageBox(NULL, "System failed to initialize", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
		LogError("System failed to initialize");
	}

	// Clean up and shutdown
	::hge->System_Shutdown();
	::hge->Release();
	return 0;
}
