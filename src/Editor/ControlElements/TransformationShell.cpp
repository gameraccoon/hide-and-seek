#include "TransformationShell.h"

#include <Debug/Log.h>

TransformationShell::TransformationShell(HGE *hge) : screenLocation(ZERO_VECTOR)
{
	this->hge = hge;

	this->texture = this->hge->Texture_Load("transform.png");

	if(!this->texture)
	{
		Log::Instance().writeError("Texture 'transform.png' not found!");
	}
	
	this->sprite = new hgeSprite(this->texture, 0, 0, 32, 32);
	this->sprite->SetColor(0xFFFFFFFF);
	this->sprite->SetHotSpot(16, 16);
}

TransformationShell::~TransformationShell(void)
{
	delete this->sprite;
	this->hge->Texture_Free(this->texture);
}

void TransformationShell::setScreenLocation(const Vector2D& scrLocation)
{
	this->screenLocation = scrLocation;
}

Vector2D TransformationShell::getScreenLocation() const
{
	return this->screenLocation;
}

void TransformationShell::render() const
{
	this->sprite->Render(this->screenLocation.x, this->screenLocation.y);
}

bool TransformationShell::click()
{
	return false;
}