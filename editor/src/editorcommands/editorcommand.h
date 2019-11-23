#ifndef EDITORCOMMAND_H
#define EDITORCOMMAND_H

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
		CommandsStack
	};

public:
	virtual ~EditorCommand() = default;
	virtual bool doCommand(World* world) = 0;
	virtual bool undoCommand(World* world) = 0;
	virtual EffectType getEffectType() = 0;
};

#endif // EDITORCOMMAND_H
