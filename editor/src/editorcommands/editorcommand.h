#pragma once

#include "Base/ComplexTypes/BitOperations.h"

#include "GameData/World.h"

class MainWindow;

class EditorCommand
{
public:
	enum class EffectType
	{
		Entities,
		Components,
		ComponentAttributes,
		CommandStack,
		SkipLayoutUpdate
	};

	using EffectBitset = Bitset<EffectType>;

public:
	EditorCommand(EffectBitset effects)
		: mEffects(effects)
	{}
	virtual ~EditorCommand() = default;

	virtual void doCommand(World* world) = 0;
	virtual void undoCommand(World* world) = 0;
	EffectBitset getEffects() { return mEffects; }

private:
	EffectBitset mEffects;
};
