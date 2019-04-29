#ifndef ABSTRACTEDITFACTORY_H
#define ABSTRACTEDITFACTORY_H

#include <memory>

#include <QObject>

#include <Core/Entity.h>
#include <Core/Component.h>

#include "src/editorcommands/editorcommandsstack.h"

class QWidget;
class QLayout;

class EditData : public QObject
{
public:
	virtual ~EditData() = default;
	virtual void fillContent(QLayout* layout, const Entity& entity, const BaseComponent* component, EditorCommandsStack& commandStack, World* world) = 0;
};

class AbstractEditFactory
{
public:
	virtual ~AbstractEditFactory() = default;
	virtual std::shared_ptr<EditData> getEditData() = 0;
};

#endif // ABSTRACTEDITFACTORY_H
