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
	ChangeEntityCommand(SetterFunction setterFunc, NullableEntity oldEntity, NullableEntity newEntity, QComboBox* affectedCombobox);

	bool doCommand(World* world) override;
	bool undoCommand(World* world) override;

private:
	NullableEntity mOldEntity;
	NullableEntity mNewEntity;
	QComboBox* mAffectedCombobox;
	SetterFunction mSetterFunc;
};

#endif // CHANGEENTITYCOMMAND_H
