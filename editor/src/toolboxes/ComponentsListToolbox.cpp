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
	mOnWorldChangedHandle = mMainWindow->OnWorldChanged.bind([this]{bindEvents(); updateContent();});
	mOnEntityChangedHandle = mMainWindow->OnSelectedEntityChanged.bind([this](NullableEntity val){onSelectedEntityChanged(val);});
}

ComponentsListToolbox::~ComponentsListToolbox()
{
	mMainWindow->OnWorldChanged.unbind(mOnWorldChangedHandle);
	mMainWindow->OnSelectedEntityChanged.unbind(mOnEntityChangedHandle);
	unbindEvents();
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

void ComponentsListToolbox::updateContent()
{
	onSelectedEntityChanged(mLastSelectedEntity);
}

void ComponentsListToolbox::onSelectedEntityChanged(NullableEntity newEntity)
{
	QListWidget* componentsList = mDockManager->findChild<QListWidget*>(ListName);
	if (componentsList == nullptr)
	{
		return;
	}

	componentsList->clear();

	World* currentWorld = mMainWindow->getCurrentWorld();

	if (currentWorld && newEntity.isValid())
	{
		unsigned int entityUid = newEntity.getEntity().getID();
		std::vector<BaseComponent*> components = currentWorld->getEntityManger().getAllEntityComponents(Entity(entityUid));
		for (auto& component : components)
		{
			componentsList->addItem(QString::fromStdString(component->getComponentTypeName()));
		}
	}

	mLastSelectedEntity = newEntity;
}

void ComponentsListToolbox::onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* /*previous*/)
{
	if (current)
	{
		mMainWindow->OnSelectedComponentChanged.broadcast(current->text());
	}
	else
	{
		mMainWindow->OnSelectedComponentChanged.broadcast("");
	}
}

void ComponentsListToolbox::bindEvents()
{
	if (World* currentWorld = mMainWindow->getCurrentWorld())
	{
		mOnComponentAddedHandle = currentWorld->getEntityManger().OnComponentAdded.bind([this]{updateContent();});
		mOnComponentRemovedHandle = currentWorld->getEntityManger().OnComponentRemoved.bind([this]{updateContent();});
	}
}

void ComponentsListToolbox::unbindEvents()
{
	if (World* currentWorld = mMainWindow->getCurrentWorld())
	{
		currentWorld->getEntityManger().OnComponentAdded.unbind(mOnComponentAddedHandle);
		currentWorld->getEntityManger().OnComponentRemoved.unbind(mOnComponentRemovedHandle);
	}
}
