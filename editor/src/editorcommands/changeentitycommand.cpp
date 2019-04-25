#include "changeentitycommand.h"

#include <QtWidgets/qcombobox.h>

#include <Core/World.h>

#include "../mainwindow.h"


ChangeEntityCommand::ChangeEntityCommand(ChangeEntityCommand::SetterFunction setterFunc, NullableEntity newEntity, NullableEntity oldEntity, QComboBox* affectedCombobox)
	: mNewEntity(newEntity)
	, mOldEntity(oldEntity)
	, mAffectedCombobox(affectedCombobox)
	, mSetterFunc(setterFunc)
{
}

void ChangeEntityCommand::doCommand(World* world, MainWindow* /*editorWindow*/)
{
	(world->*mSetterFunc)(mNewEntity);
	mAffectedCombobox->blockSignals(true);
	mAffectedCombobox->setCurrentText(QString::number(mNewEntity.mId));
	mAffectedCombobox->blockSignals(false);
}

void ChangeEntityCommand::undoCommand(World* world, MainWindow* /*editorWindow*/)
{
	(world->*mSetterFunc)(mOldEntity);
	mAffectedCombobox->blockSignals(true);
	mAffectedCombobox->setCurrentText(QString::number(mOldEntity.mId));
	mAffectedCombobox->blockSignals(false);
}
