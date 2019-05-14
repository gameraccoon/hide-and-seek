#include "EntitiesListToolbox.h"

#include "src/mainwindow.h"

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QInputDialog>

#include <Debug/Log.h>

#include "src/editorcommands/removeentitycommand.h"
#include "src/editorcommands/addcomponentcommand.h"

#include "src/toolboxes/PrefabListToolbox.h"

const QString EntitiesListToolbox::WidgetName = "EntitiesList";
const QString EntitiesListToolbox::ToolboxName = EntitiesListToolbox::WidgetName + "Toolbox";
const QString EntitiesListToolbox::ContainerName = EntitiesListToolbox::WidgetName + "Container";
const QString EntitiesListToolbox::ContainerContentName = EntitiesListToolbox::ContainerName + "Content";
const QString EntitiesListToolbox::ListName = "EntityList";

EntitiesListToolbox::EntitiesListToolbox(MainWindow* mainWindow, ads::CDockManager* dockManager)
	: mMainWindow(mainWindow)
	, mDockManager(dockManager)
{
	mOnWorldChangedHandle = mMainWindow->OnWorldChanged.bind([this]{bindEvents(); updateContent();});
	mOnSelectedEntityChangedHandle = mMainWindow->OnSelectedEntityChanged.bind([this](NullableEntity entity){onEntityChangedEvent(entity);});
}

EntitiesListToolbox::~EntitiesListToolbox()
{
	mMainWindow->OnWorldChanged.unbind(mOnWorldChangedHandle);
	mMainWindow->OnSelectedEntityChanged.unbind(mOnSelectedEntityChangedHandle);
	unbindEvents();
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
	dockWidget->setFeature(ads::CDockWidget::DockWidgetMovable, false);
	mDockManager->addDockWidget(ads::RightDockWidgetArea, dockWidget);

	containerWidget->setObjectName(ContainerName);

	QVBoxLayout* layout = new QVBoxLayout();
	containerWidget->setLayout(layout);
	QListWidget* entityList = new QListWidget();
	layout->addWidget(entityList);
	entityList->setObjectName(ListName);
	entityList->setContextMenuPolicy(Qt::CustomContextMenu);

	QObject::connect(entityList, &QListWidget::currentItemChanged, this, &EntitiesListToolbox::onCurrentItemChanged);
	QObject::connect(entityList, &QListWidget::customContextMenuRequested, this, &EntitiesListToolbox::showContextMenu);

	bindEvents();
}

void EntitiesListToolbox::onWorldUpdated()
{
	bindEvents();
}

void EntitiesListToolbox::onEntityChangedEvent(NullableEntity entity)
{
	if (!entity.isValid())
	{
		return;
	}

	QListWidget* entitiesList = mDockManager->findChild<QListWidget*>(ListName);
	if (entitiesList == nullptr)
	{
		return;
	}

	QString text = QString::number(entity.mId);
	int i = 0;
	while (QListWidgetItem* item = entitiesList->item(i))
	{
		if (item->text() == text)
		{
			entitiesList->blockSignals(true);
			entitiesList->setCurrentItem(item);
			entitiesList->blockSignals(false);
			break;
		}
		++i;
	}
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

	if (QListWidget* entitiesList = mDockManager->findChild<QListWidget*>(ListName))
	{
		entitiesList->clear();
		entitiesList->addItems(entitiesStringList);
	}
}

void EntitiesListToolbox::onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* /*previous*/)
{
	if (current)
	{
		mMainWindow->OnSelectedEntityChanged.broadcast(Entity(current->text().toUInt()));
	}
	else
	{
		mMainWindow->OnSelectedEntityChanged.broadcast(NullableEntity());
	}
}

void EntitiesListToolbox::showContextMenu(const QPoint& pos)
{
	QListWidget* entitiesList = mDockManager->findChild<QListWidget*>(ListName);
	if (entitiesList == nullptr)
	{
		return;
	}

	if (!entitiesList->currentItem())
	{
		return;
	}

	QMenu contextMenu(tr("Context menu"), this);

	QAction actionAddComponent("Add Component", this);
	connect(&actionAddComponent, &QAction::triggered, this, &EntitiesListToolbox::onAddComponentToEntityRequested);
	contextMenu.addAction(&actionAddComponent);

	QAction actionRemove("Remove Entity", this);
	connect(&actionRemove, &QAction::triggered, this, &EntitiesListToolbox::removeSelectedEntity);
	contextMenu.addAction(&actionRemove);

	QAction actionCreatePrefab("Create Prefab", this);
	connect(&actionCreatePrefab, &QAction::triggered, this, &EntitiesListToolbox::createPrefabRequested);
	contextMenu.addAction(&actionCreatePrefab);

	contextMenu.exec(entitiesList->mapToGlobal(pos));
}

void EntitiesListToolbox::removeSelectedEntity()
{
	QListWidget* entitiesList = mDockManager->findChild<QListWidget*>(ListName);
	if (entitiesList == nullptr)
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

	mMainWindow->getCommandStack().executeNewCommand<RemoveEntityCommand>(
		currentWorld,
		Entity(currentItem->text().toUInt()),
		&mMainWindow->getComponentFactory()
	);
}

void EntitiesListToolbox::createPrefabRequested()
{
	QInputDialog* dialog = new QInputDialog();
	dialog->setLabelText("Choose a name for the prefab:");
	dialog->setCancelButtonText("Cancel");
	connect(dialog, &QInputDialog::textValueSelected, this, &EntitiesListToolbox::createPrefab);
	dialog->show();
}

void EntitiesListToolbox::createPrefab(const QString& prefabName)
{
	QListWidget* entitiesList = mDockManager->findChild<QListWidget*>(ListName);
	if (entitiesList == nullptr)
	{
		return;
	}

	QListWidgetItem* currentItem = entitiesList->currentItem();
	if (currentItem == nullptr)
	{
		return;
	}

	PrefabListToolbox* prefabToolbox = mMainWindow->getPrefabToolbox();
	if (prefabToolbox == nullptr)
	{
		return;
	}

	prefabToolbox->show();
	prefabToolbox->createPrefabFromEntity(prefabName, Entity(currentItem->text().toUInt()));
}

void EntitiesListToolbox::onAddComponentToEntityRequested()
{
	QInputDialog* dialog = new QInputDialog();
	dialog->setLabelText("Select Component Type:");
	dialog->setCancelButtonText("Cancel");
	dialog->setComboBoxEditable(false);
	QStringList items;
	mMainWindow->getComponentFactory().forEachComponentType([&items](std::type_index, const std::string& name)
	{
		items.append(QString::fromStdString(name));
	});
	dialog->setComboBoxItems(items);
	connect(dialog, &QInputDialog::textValueSelected, this, &EntitiesListToolbox::addComponentToEntity);
	dialog->show();
}

void EntitiesListToolbox::addComponentToEntity(const QString& typeName)
{
	QListWidget* entitiesList = mDockManager->findChild<QListWidget*>(ListName);
	if (entitiesList == nullptr)
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

	mMainWindow->getCommandStack().executeNewCommand<AddComponentCommand>(
		currentWorld,
		Entity(currentItem->text().toUInt()),
		typeName,
		&mMainWindow->getComponentFactory()
	);
}

void EntitiesListToolbox::bindEvents()
{
	if (World* currentWorld = mMainWindow->getCurrentWorld())
	{
		mOnEntityAddedHandle = currentWorld->getEntityManger().OnEntityAdded.bind([this]{updateContent();});
		mOnEntityRemovedHandle = currentWorld->getEntityManger().OnEntityRemoved.bind([this]{updateContent();});
	}
}

void EntitiesListToolbox::unbindEvents()
{
	if (World* currentWorld = mMainWindow->getCurrentWorld())
	{
		currentWorld->getEntityManger().OnEntityAdded.unbind(mOnEntityAddedHandle);
		currentWorld->getEntityManger().OnEntityRemoved.unbind(mOnEntityRemovedHandle);
	}
}
