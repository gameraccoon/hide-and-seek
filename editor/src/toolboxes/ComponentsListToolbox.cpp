#include "ComponentsListToolbox.h"

#include "src/mainwindow.h"

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include <QVBoxLayout>

const QString ComponentsListToolbox::WidgetName = "ComponentsList";
const QString ComponentsListToolbox::ToolboxName = ComponentsListToolbox::WidgetName + "Toolbox";
const QString ComponentsListToolbox::ContainerName = ComponentsListToolbox::WidgetName + "Container";
const QString ComponentsListToolbox::ContainerContentName = ComponentsListToolbox::ContainerName + "Content";
const QString ComponentsListToolbox::ListName = "ComponentList";

ComponentsListToolbox::ComponentsListToolbox(MainWindow* mainWindow, ads::CDockManager* dockManager)
	: mMainWindow(mainWindow)
	, mDockManager(dockManager)
{
}

void ComponentsListToolbox::show()
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
	ads::CDockWidget* dockWidget = new ads::CDockWidget(QString("Components List"));
	dockWidget->setObjectName(ToolboxName);
	dockWidget->setWidget(containerWidget);
	dockWidget->setToggleViewActionMode(ads::CDockWidget::ActionModeShow);
	dockWidget->setIcon(mMainWindow->style()->standardIcon(QStyle::SP_DialogOpenButton));
	dockWidget->setFeature(ads::CDockWidget::DockWidgetClosable, true);
	mDockManager->addDockWidget(ads::RightDockWidgetArea, dockWidget);

	containerWidget->setObjectName(ContainerName);

	QVBoxLayout* layout = new QVBoxLayout();
	containerWidget->setLayout(layout);
	QListWidget* componentList = new QListWidget();
	layout->addWidget(componentList);
	componentList->setObjectName(ListName);

	QObject::connect(componentList, &QListWidget::currentItemChanged, this, &ComponentsListToolbox::onCurrentItemChanged);
}

void ComponentsListToolbox::updateSelectedComponentData(QListWidgetItem* selectedItem)
{
	bool validComponentIsSelected = false;
	if (selectedItem == nullptr)
	{
		return;
	}

	QListWidget* entitiesList = mDockManager->findChild<QListWidget*>("EntityList");
	if (entitiesList == nullptr)
	{
		return;
	}

	QWidget* componentAttributesContainerWidget = mDockManager->findChild<QWidget*>("ComponentAttributesContainer");
	if (componentAttributesContainerWidget == nullptr)
	{
		return;
	}

	QListWidgetItem* currentItem = entitiesList->currentItem();
	if (currentItem == nullptr)
	{
		return;
	}

	World* currentWorld = mMainWindow->getCurrentWorld();
	if (currentWorld == nullptr)
	{
		return;
	}

	bool ok;
	int index = currentItem->text().toInt(&ok);
	if (ok && index != 0)
	{
		Entity entity = Entity(static_cast<Entity::EntityID>(index));
		std::vector<BaseComponent*> entityComponents = currentWorld->getEntityManger().getAllEntityComponents(entity);
		auto it = std::find_if(entityComponents.begin(), entityComponents.end(), [name = selectedItem->text().toStdString()](BaseComponent* component)
		{
			return component->getComponentTypeName().compare(name) == 0;
		});

		if (it != entityComponents.end())
		{
			mMainWindow->getComponentContentFactory().replaceEditContent(componentAttributesContainerWidget->layout(), entity, *it, mMainWindow->getCommandStack(), currentWorld);
			validComponentIsSelected = true;
		}
	}

	if (!validComponentIsSelected)
	{
		mMainWindow->getComponentContentFactory().removeEditContent(componentAttributesContainerWidget->layout());
	}
}

void ComponentsListToolbox::onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* /*previous*/)
{
	updateSelectedComponentData(current);
}
