#ifndef CHANGEENTITYCOMMAND_H
#define CHANGEENTITYCOMMAND_H

#include "editorcommand.h"
#include <Core/Entity.h>

class World;
class QComboBox;

class ChangeEntityCommand : public EditorCommand
{
public:
	typedef void (World::*SetterFunction)(const NullableEntity&);

public:
	ChangeEntityCommand(SetterFunction setterFunc, NullableEntity newEntity, NullableEntity oldEntity, QComboBox* affectedCombobox);

	void doCommand(World* world, MainWindow* editorWindow) override;
	void undoCommand(World* world, MainWindow* editorWindow) override;

private:
	NullableEntity mNewEntity;
	NullableEntity mOldEntity;
	QComboBox* mAffectedCombobox;
	SetterFunction mSetterFunc;
};

#endif // CHANGEENTITYCOMMAND_H
