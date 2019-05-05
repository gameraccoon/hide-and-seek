#include "EntitiesListToolbox.h"

#include "src/mainwindow.h"

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include <QVBoxLayout>

const QString EntitiesListToolbox::WidgetName = "EntitiesList";
const QString EntitiesListToolbox::ToolboxName = EntitiesListToolbox::WidgetName + "Toolbox";
const QString EntitiesListToolbox::ContainerName = EntitiesListToolbox::WidgetName + "Container";
const QString EntitiesListToolbox::ContainerContentName = EntitiesListToolbox::ContainerName + "Content";

EntitiesListToolbox::EntitiesListToolbox(MainWindow* mainWindow, ads::CDockManager* dockManager)
	: mMainWindow(mainWindow)
	, mDockManager(dockManager)
{
}

void EntitiesListToolbox::show()
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
	ads::CDockWidget* dockWidget = new ads::CDockWidget(QString("Entities List"));
	dockWidget->setObjectName(ToolboxName);
	dockWidget->setWidget(containerWidget);
	dockWidget->setToggleViewActionMode(ads::CDockWidget::ActionModeShow);
	dockWidget->setIcon(mMainWindow->style()->standardIcon(QStyle::SP_DialogOpenButton));
	dockWidget->setFeature(ads::CDockWidget::DockWidgetClosable, false);
	mDockManager->addDockWidget(ads::RightDockWidgetArea, dockWidget);

	containerWidget->setObjectName(ContainerName);

	QVBoxLayout* layout = new QVBoxLayout();
	containerWidget->setLayout(layout);
	QListWidget* entityList = new QListWidget();
	layout->addWidget(entityList);
	entityList->setObjectName("EntityList");

	QObject::connect(entityList, &QListWidget::currentItemChanged, this, &EntitiesListToolbox::onCurrentItemChanged);
}

void EntitiesListToolbox::updateContent()
{
	World* currentWorld = mMainWindow->getCurrentWorld();
	if (currentWorld == nullptr)
	{
		return;
	}

	QStringList entitiesStringList;
	const auto& entities = currentWorld->getEntityManger().getEntities();
	for (auto& entity : entities)
	{
		entitiesStringList.append(QString::number(entity.first));
	}

	if (QListWidget* entitiesList = mDockManager->findChild<QListWidget*>("EntityList"))
	{
		entitiesList->clear();
		entitiesList->addItems(entitiesStringList);
	}
}

void EntitiesListToolbox::onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* /*previous*/)
{
	QListWidget* componentsList = mDockManager->findChild<QListWidget*>("ComponentList");
	if (componentsList == nullptr)
	{
		return;
	}

	componentsList->clear();

	World* currentWorld = mMainWindow->getCurrentWorld();

	if (current && currentWorld)
	{
		unsigned int entityUid = current->text().toUInt();
		std::vector<BaseComponent*> components = currentWorld->getEntityManger().getAllEntityComponents(Entity(entityUid));
		for (auto& component : components)
		{
			componentsList->addItem(QString::fromStdString(component->getComponentTypeName()));
		}
	}
}
