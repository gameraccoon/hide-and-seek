#include "changeentitycommand.h"

#include <QtWidgets/qcombobox.h>

#include <GameData/World.h>

#include "../mainwindow.h"


ChangeEntityCommand::ChangeEntityCommand(ChangeEntityCommand::SetterFunction setterFunc, OptionalEntity oldEntity, OptionalEntity newEntity, QComboBox* affectedCombobox)
	: mOldEntity(oldEntity)
	, mNewEntity(newEntity)
	, mAffectedCombobox(affectedCombobox)
	, mSetterFunc(setterFunc)
{
}

bool ChangeEntityCommand::doCommand(World* world)
{
	(world->*mSetterFunc)(mNewEntity);
	mAffectedCombobox->blockSignals(true);
	mAffectedCombobox->setCurrentText(QString::number(mNewEntity.mId));
	mAffectedCombobox->blockSignals(false);
	return false;
}

bool ChangeEntityCommand::undoCommand(World* world)
{
	(world->*mSetterFunc)(mOldEntity);
	mAffectedCombobox->blockSignals(true);
	mAffectedCombobox->setCurrentText(QString::number(mOldEntity.mId));
	mAffectedCombobox->blockSignals(false);
	return false;
}

EditorCommand::EffectType ChangeEntityCommand::getEffectType()
{
	return EffectType::WorldProperties;
}
