#include "../src/Globals.h"
#include "../src/Hero.h"
#include "../src/DirectionArrow.h"
#include "../src/World.h"
#include "../src/Wall.h"
#include "../src/FloatingCamera.h"
#include "../src/KeyListeners.h"
#include <vector>

// Hge subsystem
HGE *Hge = NULL;

// Pointers to the HGE objects we will use
hgeSprite*	Crosshair;
hgeFont*	Font;

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

// Class helper needed to desctruct many objects at once
class StaticGroup
{
public:
	void DestructAll()
	{
		for (std::vector<Actor*>::iterator it = Actors.begin(); it != Actors.end(); it++)
		{
			delete (*it);
			(*it) = NULL;
		}
	}

	void Insert(Actor *actor)
	{
		Actors.insert(Actors.begin(), actor);
	}

private:
	std::vector<Actor*> Actors;
};

// Handles for HGE resourcces
HTEXTURE	Texture;

Vector2D MousePos = ZeroVector;

const Vector2D SCREEN_CENTER(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

const float MAX_CAMERA_RANGE = min(SCREEN_HEIGHT, SCREEN_WIDTH) / 4.0f;

float CameraAngle = 0.0f;

// The Hero whom we control
Hero *OurHero;

// Our big World =)
World *GameWorld;

// Camera
FloatingCamera *MainCamera;

// Test arrow for show directions on screen
DirectionArrow *Arrow;

// Butto
ButtonListeners Listeners;

// event listeners
class BtnShadows : public ButtonSwitcher
{
public:
	BtnShadows(HGE *hge) : ButtonSwitcher(hge, HGEK_H, true) { };
	void Pressed() { MainCamera->ShowShadows(bActive); }
};

class BtnFog : public ButtonSwitcher
{
public:
	BtnFog(HGE *hge) : ButtonSwitcher(hge, HGEK_F, true) { };
	void Pressed() { MainCamera->ShowFog(bActive); }
};

class BtnAABB : public ButtonSwitcher
{
public:
	BtnAABB(HGE *hge) : ButtonSwitcher(hge, HGEK_C, false) { };
	void Pressed() { MainCamera->ShowAABB(bActive); }
};

class BtnHulls : public ButtonSwitcher
{
public:
	BtnHulls(HGE *hge) : ButtonSwitcher(hge, HGEK_M, false) { };
	void Pressed() { MainCamera->ShowHulls(bActive); }
};

class BtnShoot : public ButtonSwitcher
{
public:
	BtnShoot(HGE *hge) : ButtonSwitcher(hge, HGEK_LBUTTON, true) { };
	void Pressed() { OurHero->StartShoting(MainCamera->GetWorldPos(MousePos)); }
	void Released() { OurHero->StopShoting(); }
};

bool FrameFunc()
{
	float dt = Hge->Timer_GetDelta();

	// Process keys
	if (Hge->Input_GetKeyState(HGEK_ESCAPE)) return true;

	Vector2D Direction = ZeroVector;
	if (Hge->Input_GetKeyState(HGEK_A))	Direction += LeftDirection;
	if (Hge->Input_GetKeyState(HGEK_D))	Direction += RightDirection;
	if (Hge->Input_GetKeyState(HGEK_W))	Direction += UpDirection;
	if (Hge->Input_GetKeyState(HGEK_S))	Direction += DownDirection;

	if (Hge->Input_GetKeyState(HGEK_Q))	CameraAngle += 0.005f;
	if (Hge->Input_GetKeyState(HGEK_E))	CameraAngle -= 0.005f;

	OurHero->Move(Vector2D(Direction.GetRotation() - CameraAngle) * Direction.Ort().Size() * 100); // constant speed
	
	Hge->Input_GetMousePos(&MousePos.X, &MousePos.Y);

	Vector2D CameraShift((MousePos - SCREEN_CENTER)/2);
	CameraShift = CameraShift.Ort() * min(CameraShift.Size(), MAX_CAMERA_RANGE);
	MainCamera->SetLocation(OurHero->GetLocation());
	MainCamera->SetRotation(CameraAngle);
	MainCamera->SetCenterShift(CameraShift);

	// Update key states
	Listeners.Test();

	// Do World update
	GameWorld->Update(dt);

	Arrow->SetVDirection(Direction);
	Arrow->SetCenter(SCREEN_CENTER - CameraShift);

	return false;
}

bool RenderFunc()
{
	Hge->Gfx_BeginScene();
	Hge->Gfx_Clear(0xFF333333);
	//-- Here renders graphics
	MainCamera->Render();
	Crosshair->Render(MousePos.X, MousePos.Y);
	Arrow->Render();

	// fps and dt
	Font->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d", Hge->Timer_GetDelta(), Hge->Timer_GetFPS());
	
	// Status of rendering elements
	if (Listeners.GetActive(HGEK_B))
		Font->printf(5, 60, HGETEXT_LEFT, "Showing Bounding boxes");
	if (Listeners.GetActive(HGEK_C))
		Font->printf(5, 90, HGETEXT_LEFT, "Showing Models");

	if (!Listeners.GetActive(HGEK_F))
		Font->printf(5, 120, HGETEXT_LEFT, "Hidded Fog");
	if (!Listeners.GetActive(HGEK_H))
		Font->printf(5, 150, HGETEXT_LEFT, "Hidded Shadows");

	//-- end of render graphics
	Hge->Gfx_EndScene();

	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Hge = hgeCreate(HGE_VERSION);

	Hge->System_SetState(HGE_LOGFILE, "SG.log");
	Hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	Hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	Hge->System_SetState(HGE_TITLE, "Stealth game - alpha1");
	Hge->System_SetState(HGE_FPS, 100);
	Hge->System_SetState(HGE_WINDOWED, !FULL_SCREEN);
	Hge->System_SetState(HGE_SCREENWIDTH, SCREEN_WIDTH);
	Hge->System_SetState(HGE_SCREENHEIGHT, SCREEN_HEIGHT);
	Hge->System_SetState(HGE_SCREENBPP, 32);
	Hge->System_SetState(HGE_SHOWSPLASH, false); // hidding splash for develop-time only

	if(Hge->System_Initiate())
	{
		// Load sound and texture
		Texture = Hge->Texture_Load("particles.png");
		if(!Texture)
		{
			// If one of the data files is not found, display
			// an error message and shutdown.
			MessageBox(NULL, "Can't load one of the following files:\nFONT1.FNT, FONT1.PNG, PARTICLES.PNG", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			Hge->System_Shutdown();
			Hge->Release();
			return 0;
		}

		// Load a font
		Font = new hgeFont("font1.fnt");

		// Create and set up a particle system
		Crosshair = new hgeSprite(Texture, 64, 96, 32, 32);
		Crosshair->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		Crosshair->SetHotSpot(16, 16);

		GameWorld = new World(Hge);

		MainCamera = new FloatingCamera(GameWorld, Vector2D(0.0f, 0.0f));
		MainCamera->SetResolution(SCREEN_CENTER * 2);

		StaticGroup Group = StaticGroup();
		
		Group.Insert(new Wall(GameWorld, Vector2D(250.0f, 300.0f), Vector2D(80, 20)));
		Group.Insert(new Wall(GameWorld, Vector2D(250.0f, 200.0f), Vector2D(80, 20)));
		Group.Insert(new Wall(GameWorld, Vector2D(200.0f, 250.0f), Vector2D(20, 80)));
		//Group.Insert(new Wall(GameWorld, Vector2D(500.0f, 450.0f), Vector2D(20, 80)));

		Group.Insert(new Wall(GameWorld, Vector2D(450.0f, 300.0f), Vector2D(80, 20)));
		Group.Insert(new Wall(GameWorld, Vector2D(450.0f, 200.0f), Vector2D(80, 20)));
		//Group.Insert(new Wall(GameWorld, Vector2D(400.0f, 250.0f), Vector2D(20, 80)));
		Group.Insert(new Wall(GameWorld, Vector2D(500.0f, 250.0f), Vector2D(20, 80)));
		
		//Group.Insert(new Wall(GameWorld, Vector2D(350.0f, 200.0f), Vector2D(80, 20)));
		//Group.Insert(new Wall(GameWorld, Vector2D(350.0f, 100.0f), Vector2D(80, 20)));
		Group.Insert(new Wall(GameWorld, Vector2D(300.0f, 150.0f), Vector2D(20, 80)));
		Group.Insert(new Wall(GameWorld, Vector2D(400.0f, 150.0f), Vector2D(20, 80)));
		
		//Group.Insert(new Wall(GameWorld, Vector2D(350.0f, 400.0f), Vector2D(80, 20)));
		//Group.Insert(new Wall(GameWorld, Vector2D(350.0f, 300.0f), Vector2D(80, 20)));
		Group.Insert(new Wall(GameWorld, Vector2D(300.0f, 350.0f), Vector2D(20, 80)));
		Group.Insert(new Wall(GameWorld, Vector2D(400.0f, 350.0f), Vector2D(20, 80)));

		OurHero = new Hero(GameWorld, Vector2D(0.0f, 350.0f));
		Group.Insert(OurHero);

		OurHero->GiveWeapon(new Weapon());

		Arrow = new DirectionArrow(Hge);
		Arrow->SetCenter(SCREEN_CENTER);
		
		Listeners.AddListener(new BtnAABB(Hge));
		Listeners.AddListener(new BtnHulls(Hge));
		Listeners.AddListener(new BtnFog(Hge));
		Listeners.AddListener(new BtnShadows(Hge));
		Listeners.AddListener(new BtnShoot(Hge));

		// Let's rock now!
		Hge->System_Start();

		// Delete created objects and free loaded resources
		delete Font;
		delete Crosshair;
		delete Arrow;
		Group.DestructAll();
		delete MainCamera;
		delete GameWorld;
		Hge->Texture_Free(Texture);
	}
	else
	{
		MessageBox(NULL, "System failed to initialize", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
	}

	// Clean up and shutdown
	Hge->System_Shutdown();
	Hge->Release();
	return 0;
}
