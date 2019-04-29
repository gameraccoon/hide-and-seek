#ifndef CHANGETRANSFORMCOMPONENTLOCATIONCOMMAND_H
#define CHANGETRANSFORMCOMPONENTLOCATIONCOMMAND_H

#include "editorcommand.h"

#include <Core/Vector2D.h>

class ChangeTransformComponentLocationCommand : public EditorCommand
{
public:
	ChangeTransformComponentLocationCommand(const Entity& entity, const Vector2D& oldLocation, const Vector2D& newLocation);

	void doCommand(World* world) override;
	void undoCommand(World* world) override;

private:
	Entity mEntity;
	Vector2D mOldLocation;
	Vector2D mNewLocation;
};

#endif // CHANGETRANSFORMCOMPONENTLOCATIONCOMMAND_H
