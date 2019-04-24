#ifndef EDITORCOMMAND_H
#define EDITORCOMMAND_H

#include <Core/World.h>

class MainWindow;

class EditorCommand
{
public:
	virtual ~EditorCommand() = default;
	virtual void Do(World* world, MainWindow* editorWindow) = 0;
	virtual void Undo(World* world, MainWindow* editorWindow) = 0;
};

#endif // EDITORCOMMAND_H
