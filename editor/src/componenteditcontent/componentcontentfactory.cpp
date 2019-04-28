#include "componentcontentfactory.h"

#include <QPushButton>
#include <QSpacerItem>

#include "componentregistration.h"

void ComponentContentFactory::registerComponents()
{
	ComponentRegistration::RegisterToEditFactory(mFactories);
}

void ComponentContentFactory::replaceEditContent(QLayout* layout, const BaseComponent* component)
{
	auto it = mFactories.find(component->getComponentTypeName());

	QWidget* newContent = nullptr;
	if (it != mFactories.end())
	{
		mCurrentEdit = it->second->getEditData();
		newContent = mCurrentEdit->newContentWidget(component);
	}

	if (newContent != nullptr && mContentWidget == nullptr)
	{
		layout->addWidget(newContent);
		newContent->show();
	}
	else if (newContent != nullptr && mContentWidget == nullptr)
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
