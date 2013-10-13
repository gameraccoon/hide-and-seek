#include "BaseProperties.h"

HGE *hge=0;

// Pointers to the HGE objects we will use
hgeSprite*	Crosshair;
hgeFont*	Font;

// Handles for HGE resourcces
HEFFECT		snd;
HTEXTURE	tex;

Vector2D MousePos(0.0f, 0.0f);

class Hero
{
public:
	Vector2D Location;
	Vector2D CurrentSpeed;
	float Speed;

	Hero(Vector2D location) : Location(location), CurrentSpeed(0.0f, 0.0f), NewLocation(location)
	{
		Speed = 1200.0f;

		Texture = hge->Texture_Load("particles.png");

		WARN_IF(!Texture, "Texture 'particles.png' not found!");
		
		HeroSprite = new hgeSprite(Texture, 96, 64, 32, 32);
		HeroSprite->SetColor(0xFFFFA000);
		HeroSprite->SetHotSpot(16, 16);
	}

	~Hero()
	{
		delete HeroSprite;
		hge->Texture_Free(Texture);
	}

	void Move(Vector2D step)
	{
		Location += step;
	}

	void Update(float deltaTime)
	{
	}

	void Render()
	{
		HeroSprite->Render(Location.X, Location.Y);
	}

private:
	Vector2D NewLocation;
	hgeSprite* HeroSprite;
	HTEXTURE Texture;
};

// Initialze Hero
Hero *OurHero;

// Play sound effect
void boom(float dt) {
	int pan = int((OurHero->Location.X - 400) / 4);
	float pitch = OurHero->CurrentSpeed.Size() * dt * 0.005f + 0.2f;
	float power = OurHero->CurrentSpeed.Size() * dt * 3.0f + 30.0f;
	hge->Effect_PlayEx(snd, power, pan, pitch);
}

bool FrameFunc()
{
	float dt = hge->Timer_GetDelta();

	// Process keys
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;

	Vector2D Direction(0.0f, 0.0f);
	if (hge->Input_GetKeyState(HGEK_A))	Direction += LeftDirection;
	if (hge->Input_GetKeyState(HGEK_D))	Direction += RightDirection;
	if (hge->Input_GetKeyState(HGEK_W))	Direction += UpDirection;
	if (hge->Input_GetKeyState(HGEK_S))	Direction += DownDirection;

	OurHero->Move(Direction.Ort() * 5); // constant speed
	
	hge->Input_GetMousePos(&MousePos.X, &MousePos.Y);

	// Do some movement calculations	
	OurHero->Update(dt);

	// Collision detection
	if (OurHero->Location.X > 784)
	{
		OurHero->Location.X = 784 - (OurHero->Location.X - 784);
		OurHero->CurrentSpeed.X = -OurHero->CurrentSpeed.X;
		boom(dt);
	}
	
	if (OurHero->Location.X < 16)
	{
		OurHero->Location.X = 16 + 16 - OurHero->Location.X;
		OurHero->CurrentSpeed.X = -OurHero->CurrentSpeed.X;
		boom(dt);
	}

	if (OurHero->Location.Y > 584)
	{
		OurHero->Location.Y = 584 - (OurHero->Location.Y - 584);
		OurHero->CurrentSpeed.Y = -OurHero->CurrentSpeed.Y;
		boom(dt);
	}

	if (OurHero->Location.Y < 16)
	{
		OurHero->Location.Y = 16 + 16 - OurHero->Location.Y;
		OurHero->CurrentSpeed.Y = -OurHero->CurrentSpeed.Y;
		boom(dt);
	}

	return false;
}

bool RenderFunc()
{
	// Render graphics
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);
	Crosshair->Render(MousePos.X, MousePos.Y);
	OurHero->Render();
	Font->printf(5, 5, HGETEXT_LEFT, "dt:%.3f\nFPS:%d (constant)", hge->Timer_GetDelta(), hge->Timer_GetFPS());
	hge->Gfx_EndScene();

	return false;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_LOGFILE, "SG.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "Stealth game - alpha1");
	hge->System_SetState(HGE_FPS, 100);
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, 800);
	hge->System_SetState(HGE_SCREENHEIGHT, 600);
	hge->System_SetState(HGE_SCREENBPP, 32);

	if(hge->System_Initiate())
	{
		// Load sound and texture
		snd=hge->Effect_Load("menu.wav");
		tex=hge->Texture_Load("particles.png");
		if(!snd || !tex)
		{
			// If one of the data files is not found, display
			// an error message and shutdown.
			MessageBox(NULL, "Can't load one of the following files:\nMENU.WAV, FONT1.FNT, FONT1.PNG, PARTICLES.PNG, TRAIL.PSI", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			hge->System_Shutdown();
			hge->Release();
			return 0;
		}

		// Load a font
		Font = new hgeFont("font1.fnt");

		// Create and set up a particle system
		Crosshair = new hgeSprite(tex, 64, 96, 32, 32);
		Crosshair->SetBlendMode(BLEND_COLORMUL | BLEND_ALPHAADD | BLEND_NOZWRITE);
		Crosshair->SetHotSpot(16,16);

		OurHero = new Hero(Vector2D(100.0f, 100.0f));

		// Let's rock now!
		hge->System_Start();

		// Delete created objects and free loaded resources
		delete Font;
		delete Crosshair;
		delete OurHero;
		hge->Texture_Free(tex);
		hge->Effect_Free(snd);
	}

	// Clean up and shutdown
	hge->System_Shutdown();
	hge->Release();
	return 0;
}
