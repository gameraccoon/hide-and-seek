#ifndef EDITORCOMMAND_H
#define EDITORCOMMAND_H

#include <Core/World.h>

class MainWindow;

class EditorCommand
{
public:
	virtual ~EditorCommand() = default;
	virtual void doCommand(World* world) = 0;
	virtual void undoCommand(World* world) = 0;
};

#endif // EDITORCOMMAND_H
