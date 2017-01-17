#define GLM_FORCE_RADIANS

#include "StealthGame.h"

namespace Game
{
	void StealthGame::Start()
	{
		// start the main loop
		getEngine()->Start(*this);
	}

	void StealthGame::Update(float )
	{
	}

	void StealthGame::Draw()
	{
		std::unique_ptr<Texture> text3;
		{
			Texture text1 = getResourceManager()->getTexture("arrow.png");
			Texture text2 = text1;
			text3.reset(new Texture(text2));

			text1.Draw(100, 200);
			text2.Draw(200, 200);
		}

		text3->Draw(100, 300);
	}
}
