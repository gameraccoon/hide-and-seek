#ifndef CHANGETRANSFORMCOMPONENTROTATIONCOMMAND_H
#define CHANGETRANSFORMCOMPONENTROTATIONCOMMAND_H

#include "editorcommand.h"

#include "src/componenteditcontent/typeeditconstructorhelpers.h"

class ChangeTransformComponentRotationCommand : public EditorCommand
{
public:
	ChangeTransformComponentRotationCommand(const Entity& entity, float oldRotation, float newRotataion);

	void doCommand(World* world) override;
	void undoCommand(World* world) override;

private:
	Entity mEntity;
	float mOldRotation;
	float mNewRotation;
};

#endif // CHANGETRANSFORMCOMPONENTROTATIONCOMMAND_H
