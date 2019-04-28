#ifndef TRANSFORMCOMPONENTEDITFACTORY_H
#define TRANSFORMCOMPONENTEDITFACTORY_H

#include "abstracteditfactory.h"

class TransformComponentEditData : public EditData
{
public:
	QWidget* newContentWidget(const BaseComponent* component) override;
};

class TransformComponentEditFactory : public AbstractEditFactory
{
public:
	std::shared_ptr<EditData> getEditData() override;
};

#endif // TRANSFORMCOMPONENTEDITFACTORY_H
