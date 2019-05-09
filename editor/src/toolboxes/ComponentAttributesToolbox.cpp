#include "ComponentAttributesToolbox.h"

#include "src/mainwindow.h"

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include <QVBoxLayout>

const QString ComponentAttributesToolbox::WidgetName = "ComponentAttributes";
const QString ComponentAttributesToolbox::ToolboxName = ComponentAttributesToolbox::WidgetName + "Toolbox";
const QString ComponentAttributesToolbox::ContainerName = ComponentAttributesToolbox::WidgetName + "Container";
const QString ComponentAttributesToolbox::ContainerContentName = ComponentAttributesToolbox::ContainerName + "Content";

ComponentAttributesToolbox::ComponentAttributesToolbox(MainWindow* mainWindow, ads::CDockManager* dockManager)
	: mMainWindow(mainWindow)
	, mDockManager(dockManager)
{
	mOnComponentChangedHandle = mMainWindow->OnSelectedComponentChanged.bind([this](const QString& val){onSelectedComponentChange(val);});
	mOnComponentContentChangedHandle = mMainWindow->OnComponentContentChange.bind([this]{updateContent();});
}

ComponentAttributesToolbox::~ComponentAttributesToolbox()
{
	mMainWindow->OnSelectedComponentChanged.unbind(mOnComponentChangedHandle);
	mMainWindow->OnComponentContentChange.unbind(mOnComponentContentChangedHandle);
}

void ComponentAttributesToolbox::show()
{
	if (ads::CDockWidget* dockWidget = mDockManager->findDockWidget(ToolboxName))
	{
		if (dockWidget->isVisible())
		{
			return;
		}
		else
		{
			mDockManager->layout()->removeWidget(dockWidget);
		}
	}

	QWidget* containerWidget = new QWidget();
	ads::CDockWidget* dockWidget = new ads::CDockWidget(QString("Component Attributes"));
	dockWidget->setObjectName(ToolboxName);
	dockWidget->setWidget(containerWidget);
	dockWidget->setToggleViewActionMode(ads::CDockWidget::ActionModeShow);
	dockWidget->setIcon(mMainWindow->style()->standardIcon(QStyle::SP_DialogOpenButton));
	dockWidget->setFeature(ads::CDockWidget::DockWidgetClosable, true);
	mDockManager->addDockWidget(ads::RightDockWidgetArea, dockWidget);

	containerWidget->setObjectName(ContainerName);

	QVBoxLayout* layout = new QVBoxLayout();
	containerWidget->setLayout(layout);
}

void ComponentAttributesToolbox::updateContent()
{
	onSelectedComponentChange(mLastSelectedComlonent);
}

void ComponentAttributesToolbox::clearContent()
{
	QWidget* componentAttributesContainerWidget = mDockManager->findChild<QWidget*>(ContainerName);
	if (componentAttributesContainerWidget == nullptr)
	{
		return;
	}
	mMainWindow->getComponentContentFactory().removeEditContent(componentAttributesContainerWidget->layout());
}

void ComponentAttributesToolbox::onSelectedComponentChange(const QString& componentTypeName)
{
	QListWidget* entitiesList = mDockManager->findChild<QListWidget*>("EntityList");
	if (entitiesList == nullptr)
	{
		return;
	}

	QWidget* componentAttributesContainerWidget = mDockManager->findChild<QWidget*>(ContainerName);
	if (componentAttributesContainerWidget == nullptr)
	{
		return;
	}

	QListWidgetItem* currentItem = entitiesList->currentItem();
	if (currentItem == nullptr)
	{
		clearContent();
		return;
	}

	World* currentWorld = mMainWindow->getCurrentWorld();
	if (currentWorld == nullptr)
	{
		return;
	}

	bool validComponentIsSelected = false;

	bool ok;
	int index = currentItem->text().toInt(&ok);
	if (ok && index != 0)
	{
		Entity entity = Entity(static_cast<Entity::EntityID>(index));
		std::vector<BaseComponent*> entityComponents = currentWorld->getEntityManger().getAllEntityComponents(entity);
		auto it = std::find_if(entityComponents.begin(), entityComponents.end(), [componentTypeName = componentTypeName.toStdString()](BaseComponent* component)
		{
			return component->getComponentTypeName().compare(componentTypeName) == 0;
		});

		if (it != entityComponents.end())
		{
			mMainWindow->getComponentContentFactory().replaceEditContent(componentAttributesContainerWidget->layout(), entity, *it, mMainWindow->getCommandStack(), currentWorld);
			validComponentIsSelected = true;
			mLastSelectedComlonent = componentTypeName;
		}
	}

	if (!validComponentIsSelected)
	{
		clearContent();
	}
}
