#include "WorldPropertiesToolbox.h"

#include "src/mainwindow.h"

#include "src/editorcommands/changeentitycommand.h"

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QStringList>

const QString WorldPropertiesToolbox::WidgetName = "WorldProperties";
const QString WorldPropertiesToolbox::ToolboxName = WorldPropertiesToolbox::WidgetName + "Toolbox";
const QString WorldPropertiesToolbox::ContainerName = WorldPropertiesToolbox::WidgetName + "Container";
const QString WorldPropertiesToolbox::ContainerContentName = WorldPropertiesToolbox::ContainerName + "Content";

WorldPropertiesToolbox::WorldPropertiesToolbox(MainWindow* mainWindow, ads::CDockManager* dockManager)
	: mMainWindow(mainWindow)
	, mDockManager(dockManager)
{
	mOnWorldChangedHandle = mMainWindow->OnWorldChanged.bind([this]{updateContent();});
}

WorldPropertiesToolbox::~WorldPropertiesToolbox()
{
	mMainWindow->OnWorldChanged.unbind(mOnWorldChangedHandle);
}

void WorldPropertiesToolbox::show()
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
	ads::CDockWidget* dockWidget = new ads::CDockWidget(QString("World Properties"));
	dockWidget->setObjectName(ToolboxName);
	dockWidget->setWidget(containerWidget);
	dockWidget->setToggleViewActionMode(ads::CDockWidget::ActionModeShow);
	dockWidget->setIcon(mMainWindow->style()->standardIcon(QStyle::SP_DialogOpenButton));
	dockWidget->setFeature(ads::CDockWidget::DockWidgetClosable, true);
	mDockManager->addDockWidget(ads::RightDockWidgetArea, dockWidget);

	containerWidget->setObjectName(ContainerName);

	QVBoxLayout* layout = new QVBoxLayout();
	containerWidget->setLayout(layout);
	QWidget* content = new QWidget();
	content->setObjectName(ContainerContentName);
	layout->addWidget(content);
}

void WorldPropertiesToolbox::updateContent()
{
	World* currentWorld = mMainWindow->getCurrentWorld();
	if (currentWorld == nullptr)
	{
		return;
	}

	QStringList entitiesStringList;
	const auto& entities = currentWorld->getEntityManager().getEntities();
	for (auto& entity : entities)
	{
		entitiesStringList.append(QString::number(entity.first));
	}

	QWidget* worldSettingsContainerWidget = mDockManager->findChild<QWidget*>(ContainerName);
	if (worldSettingsContainerWidget == nullptr)
	{
		return;
	}

	QWidget* content = new QWidget();
	QVBoxLayout* worldSettingsLayout = new QVBoxLayout();
	content->setLayout(worldSettingsLayout);

	{
		worldSettingsLayout->addWidget(new QLabel("Controlled Entity"));
		QComboBox* controlledEntityCombobox = new QComboBox();
		controlledEntityCombobox->setObjectName("ControlledEntityCombobox");
		worldSettingsLayout->addWidget(controlledEntityCombobox);
		controlledEntityCombobox->addItems(entitiesStringList);
		if (OptionalEntity controlledEntity = currentWorld->getPlayerControlledEntity(); controlledEntity.isValid())
		{
			controlledEntityCombobox->setCurrentText(QString::number(controlledEntity.mId));
		}
		QObject::connect(controlledEntityCombobox, &QComboBox::currentTextChanged, this, &WorldPropertiesToolbox::onControledEntityChanged);
	}

	{
		worldSettingsLayout->addWidget(new QLabel("Camera Entity"));
		QComboBox* cameraEntityCombobox = new QComboBox();
		cameraEntityCombobox->setObjectName("CameraEntityCombobox");
		worldSettingsLayout->addWidget(cameraEntityCombobox);
		cameraEntityCombobox->addItems(entitiesStringList);
		if (OptionalEntity camera = currentWorld->getMainCamera(); camera.isValid())
		{
			cameraEntityCombobox->setCurrentText(QString::number(camera.mId));
		}
		QObject::connect(cameraEntityCombobox, &QComboBox::currentTextChanged, this, &WorldPropertiesToolbox::onCameraEntityChanged);
	}

	worldSettingsLayout->addStretch();

	QWidget* oldContent = worldSettingsContainerWidget->findChild<QWidget*>(ContainerContentName);
	content->setObjectName(ContainerContentName);
	worldSettingsContainerWidget->layout()->replaceWidget(oldContent, content);
	delete oldContent;
}

void WorldPropertiesToolbox::onControledEntityChanged(const QString& arg1)
{
	World* currentWorld = mMainWindow->getCurrentWorld();
	if (currentWorld == nullptr)
	{
		return;
	}

	OptionalEntity entity;
	int index = arg1.toInt();
	if (index != 0)
	{
		entity = Entity(static_cast<Entity::EntityID>(index));
	}

	QComboBox* controlledEntityCombobox = mDockManager->findChild<QComboBox*>("ControlledEntityCombobox");
	mMainWindow->getCommandStack().executeNewCommand<ChangeEntityCommand>(currentWorld,
		&World::setPlayerControlledEntity,
		currentWorld->getPlayerControlledEntity(),
		entity,
		controlledEntityCombobox);
}

void WorldPropertiesToolbox::onCameraEntityChanged(const QString& arg1)
{
	World* currentWorld = mMainWindow->getCurrentWorld();
	if (currentWorld == nullptr)
	{
		return;
	}

	OptionalEntity entity;
	int index = arg1.toInt();
	if (index != 0)
	{
		entity = Entity(static_cast<Entity::EntityID>(index));
	}

	QComboBox* cameraEntityCombobox = mDockManager->findChild<QComboBox*>("CameraEntityCombobox");
	mMainWindow->getCommandStack().executeNewCommand<ChangeEntityCommand>(currentWorld,
		&World::setMainCamera,
		currentWorld->getMainCamera(),
		entity,
		cameraEntityCombobox);
}
