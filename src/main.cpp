#include "../src/Globals.h"
#include "../src/Hero.h"

// Pointers to the HGE objects we will use
hgeSprite*	Crosshair;
hgeFont*	Font;

// Handles for HGE resourcces
HTEXTURE	Texture;

Vector2D MousePos = ZeroVector;

// Initialze Hero
Hero *OurHero;

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

	OurHero->Move(Direction.Ort() * 500); // constant speed
	
	Hge->Input_GetMousePos(&MousePos.X, &MousePos.Y);

	// Do some movement calculations for Hero
	OurHero->Update(dt);

	return false;
}

bool RenderFunc()
{
	Hge->Gfx_BeginScene();
	Hge->Gfx_Clear(0);
	//-- Here renders graphics
	Crosshair->Render(MousePos.X, MousePos.Y);
	OurHero->Render();
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
	Hge->System_SetState(HGE_WINDOWED, true);
	Hge->System_SetState(HGE_SCREENWIDTH, 800);
	Hge->System_SetState(HGE_SCREENHEIGHT, 600);
	Hge->System_SetState(HGE_SCREENBPP, 32);

	if(Hge->System_Initiate())
	{
		// Load sound and texture
		Texture = Hge->Texture_Load("particles.png");
		if(!Texture)
		{
			// If one of the data files is not found, display
			// an error message and shutdown.
			MessageBox(NULL, "Can't load one of the following files:\nFONT1.FNT, FONT1.PNG, PARTICLES.PNG, TRAIL.PSI", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			Hge->System_Shutdown();
			Hge->Release();
			return 0;
		}

		// Load a font
		Font = new hgeFont("font1.fnt");

		// Create and set up a particle system
		Crosshair = new hgeSprite(Texture, 64, 96, 32, 32);
		Crosshair->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		Crosshair->SetHotSpot(16,16);

		OurHero = new Hero(Vector2D(100.0f, 100.0f));

		// Let's rock now!
		Hge->System_Start();

		// Delete created objects and free loaded resources
		delete Font;
		delete Crosshair;
		delete OurHero;
		Hge->Texture_Free(Texture);
	}

	// Clean up and shutdown
	Hge->System_Shutdown();
	Hge->Release();
	return 0;
}
