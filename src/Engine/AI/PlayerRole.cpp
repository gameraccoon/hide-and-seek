#include "PlayerRole.h"

PlayerRole::PlayerRole(World* world, IBody *body)
	: Role(world, body)
{
}

PlayerRole::~PlayerRole()
{
}

void PlayerRole::update(float )
{
}

void PlayerRole::onTakeDamage(IActor* , float , Vector2D )
{
}

void PlayerRole::onSeeEnemy(IActor *)
{
}

void PlayerRole::onHearNoise(SoundVolume *)
{
}

void PlayerRole::gotoState(std::string )
{
}

void PlayerRole::endCurrentState()
{
}