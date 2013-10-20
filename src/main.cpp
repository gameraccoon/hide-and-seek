#include "../src/Globals.h"
#include "../src/Hero.h"
#include "../src/DirectionArrow.h"
#include "../src/World.h"
#include "../src/Wall.h"
#include "../src/FloatingCamera.h"
#include <vector>

// Hge subsystem
HGE *Hge = NULL;

// Pointers to the HGE objects we will use
hgeSprite*	Crosshair;
hgeFont*	Font;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const bool bWINDOWED = true;

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

bool bShowCollision = false;
bool bBtnCPressed = false;

bool bShowFog = true;
bool bBtnFPressed = false;

Vector2D MousePos = ZeroVector;

const Vector2D SCREEN_CENTER(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

const float MAX_CAMERA_RANGE = min(SCREEN_HEIGHT, SCREEN_WIDTH) / 4.0f;

float CameraAngle = 0.0f;

// The Hero whom we control
Hero *OurHero;

// Our big World =)
World *GameWorld;

//
FloatingCamera *MainCamera;

// Test arrow for show directions on screen
DirectionArrow *Arrow;

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
	
	// Switch on/off showing collizion boxes
	if (Hge->Input_GetKeyState(HGEK_C))
	{
		if (!bBtnCPressed)
		{
			bBtnCPressed = true;
			bShowCollision = !bShowCollision;
			MainCamera->ShowCollision(bShowCollision);
		}
	}
	else
	{
		bBtnCPressed = false;
	}

	// Switch on/off showing Fog
	if (Hge->Input_GetKeyState(HGEK_F))
	{
		if (!bBtnFPressed)
		{
			bBtnFPressed = true;
			bShowFog = !bShowFog;
			MainCamera->ShowFog(bShowFog);
		}
	}
	else
	{
		bBtnFPressed = false;
	}

	// Do World update
	GameWorld->Update(dt);

	Arrow->SetVDirection(Direction);
	Arrow->SetCenter(SCREEN_CENTER - CameraShift);

	return false;
}

bool RenderFunc()
{
	Hge->Gfx_BeginScene();
	Hge->Gfx_Clear(0);
	//-- Here renders graphics
	MainCamera->Render();
	Crosshair->Render(MousePos.X, MousePos.Y);
	Arrow->Render();
	Font->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d (constant)", Hge->Timer_GetDelta(), Hge->Timer_GetFPS());
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
	Hge->System_SetState(HGE_WINDOWED, bWINDOWED);
	Hge->System_SetState(HGE_SCREENWIDTH, SCREEN_WIDTH);
	Hge->System_SetState(HGE_SCREENHEIGHT, SCREEN_HEIGHT);
	Hge->System_SetState(HGE_SCREENBPP, 32);

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

		GameWorld = new World();

		MainCamera = new FloatingCamera(Hge, GameWorld, Vector2D(0.0f, 0.0f));
		MainCamera->SetResolution(SCREEN_CENTER * 2);

		StaticGroup Group = StaticGroup();
		
		Group.Insert(new Wall(GameWorld, Hge, Vector2D(250.0f, 300.0f), Vector2D(80, 20)));
		Group.Insert(new Wall(GameWorld, Hge, Vector2D(250.0f, 200.0f), Vector2D(80, 20)));
		Group.Insert(new Wall(GameWorld, Hge, Vector2D(200.0f, 250.0f), Vector2D(20, 80)));
		Group.Insert(new Wall(GameWorld, Hge, Vector2D(300.0f, 250.0f), Vector2D(20, 80)));

		OurHero = new Hero(GameWorld, Hge, Vector2D(0.0f, 0.0f));
		Group.Insert(OurHero);

		Arrow = new DirectionArrow(Hge);
		Arrow->SetCenter(SCREEN_CENTER);

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
