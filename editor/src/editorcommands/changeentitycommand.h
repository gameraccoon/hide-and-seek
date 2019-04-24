#ifndef CHANGEENTITYCOMMAND_H
#define CHANGEENTITYCOMMAND_H

#include "editorcommand.h"
#include <Core/Entity.h>

class World;
class QComboBox;

class ChangeEntityCommand : public EditorCommand
{
public:


public:
	ChangeEntityCommand(NullableEntity newEntity, NullableEntity oldEntity, QComboBox* affectedCombobox);
	void Do(World* world, MainWindow* editorWindow) override;
	void Undo(World* world, MainWindow* editorWindow) override;

private:
	NullableEntity mNewEntity;
	NullableEntity mOldEntity;
	QComboBox* mAffectedCombobox;
};

#endif // CHANGEENTITYCOMMAND_H
