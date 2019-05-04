#ifndef EDITORCOMMAND_H
#define EDITORCOMMAND_H

#include <Core/World.h>

class MainWindow;

class EditorCommand
{
public:
	virtual ~EditorCommand() = default;
	virtual bool doCommand(World* world) = 0;
	virtual bool undoCommand(World* world) = 0;
};

#endif // EDITORCOMMAND_H
