#ifndef CHANGEENTITYCOMMAND_H
#define CHANGEENTITYCOMMAND_H

#include "editorcommand.h"
#include "ECS/Entity.h"

class World;
class QComboBox;

class ChangeEntityCommand : public EditorCommand
{
public:
	using SetterFunction = void (World::*)(const OptionalEntity&);

public:
	ChangeEntityCommand(SetterFunction setterFunc, OptionalEntity oldEntity, OptionalEntity newEntity, QComboBox* affectedCombobox);

	bool doCommand(World* world) override;
	bool undoCommand(World* world) override;
	EffectType getEffectType() override;

private:
	OptionalEntity mOldEntity;
	OptionalEntity mNewEntity;
	QComboBox* mAffectedCombobox;
	SetterFunction mSetterFunc;
};

#endif // CHANGEENTITYCOMMAND_H
