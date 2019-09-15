#pragma once

class World;
class GameData;

struct WorldHolder
{
	World* world = nullptr;
	GameData* gameData = nullptr;
};
