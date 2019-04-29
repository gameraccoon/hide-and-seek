#ifndef TRANSFORMCOMPONENTEDITFACTORY_H
#define TRANSFORMCOMPONENTEDITFACTORY_H

#include "abstracteditfactory.h"

#include "src/componenteditcontent/typeeditconstructorhelpers.h"

#include <Core/Vector2D.h>

class TransformComponentEditData : public EditData
{
public:
	void fillContent(QLayout* layout, const Entity& entity, const BaseComponent* component, EditorCommandsStack& commandStack, World* world) override;

private:
	TypesEditConstructor::Edit<Vector2D>::Ptr mLocationEdit;
	TypesEditConstructor::Edit<float>::Ptr mRotationEdit;
};

class TransformComponentEditFactory : public AbstractEditFactory
{
public:
	std::shared_ptr<EditData> getEditData() override;
};

#endif // TRANSFORMCOMPONENTEDITFACTORY_H
