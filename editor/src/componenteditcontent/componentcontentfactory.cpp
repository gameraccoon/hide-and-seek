#include "componentcontentfactory.h"

#include <QPushButton>
#include <QSpacerItem>

#include "componentregistration.h"

#include "src/editorutils/componentreference.h"

void ComponentContentFactory::registerComponents()
{
	ComponentRegistration::RegisterToEditFactory(mFactories);
}

void ComponentContentFactory::replaceEditContent(QLayout* layout, const ComponentSourceReference& sourceReference, const BaseComponent* component, EditorCommandsStack& commandStack, World* world)
{
	auto it = mFactories.find(component->getComponentTypeName());

	QWidget* newContent = nullptr;
	if (it != mFactories.end())
	{
		mCurrentEdit = it->second->getEditData();
		newContent = new QWidget();
		QVBoxLayout* innerLayout = new QVBoxLayout();
		mCurrentEdit->fillContent(innerLayout, sourceReference, component, commandStack, world);
		innerLayout->addStretch();
		newContent->setLayout(innerLayout);
	}
	else
	{
		mCurrentEdit = nullptr;
		ReportError("ComponentEditFactory not registered for component type '%s'", component->getComponentTypeName());
	}

	if (newContent != nullptr && mContentWidget == nullptr)
	{
		layout->addWidget(newContent);
		newContent->show();
	}
	else if (newContent != nullptr && mContentWidget != nullptr)
	{
		mContentWidget->hide();
		layout->replaceWidget(mContentWidget, newContent);
		newContent->show();
		delete mContentWidget;
	}
	else if (mContentWidget != nullptr)
	{
		mContentWidget->hide();
		layout->removeWidget(mContentWidget);
		delete mContentWidget;
	}

	mContentWidget = newContent;
}

void ComponentContentFactory::removeEditContent(QLayout* layout)
{
	if (mContentWidget)
	{
		mContentWidget->hide();
		layout->removeWidget(mContentWidget);
		delete mContentWidget;
		mContentWidget = nullptr;
	}
}
