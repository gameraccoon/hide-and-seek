#include "changeentitycommand.h"

#include <QtWidgets/qcombobox.h>

#include <Core/World.h>

#include "../mainwindow.h"

ChangeEntityCommand::ChangeEntityCommand(NullableEntity newEntity, NullableEntity oldEntity, QComboBox* affectedCombobox)
	: mNewEntity(newEntity)
	, mOldEntity(oldEntity)
	, mAffectedCombobox(affectedCombobox)
{
}

void ChangeEntityCommand::Do(World* world, MainWindow* editorWindow)
{
}

void ChangeEntityCommand::Undo(World* world, MainWindow* editorWindow)
{
}
