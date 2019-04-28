#ifndef ABSTRACTEDITFACTORY_H
#define ABSTRACTEDITFACTORY_H

#include <memory>

#include <QObject>

#include <Core/Component.h>

class QWidget;

class EditData : public QObject
{
public:
	virtual ~EditData() = default;
	virtual QWidget* newContentWidget(const BaseComponent* component) = 0;
};

class AbstractEditFactory
{
public:
	virtual ~AbstractEditFactory() = default;
	virtual std::shared_ptr<EditData> getEditData() = 0;
};

#endif // ABSTRACTEDITFACTORY_H
