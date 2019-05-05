#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "src/editorcommands/changeentitycommand.h"
#include "src/componenteditcontent/componentcontentfactory.h"

#include "src/componenteditcontent/componentregistration.h"

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include <Core/World.h>
#include <Modules/WorldLoader.h>

#include <QFileDialog>
#include <QComboBox>
#include <QProcess>
#include <QLabel>
#include <QLineEdit>
#include <QCalendarWidget>

#include <Components/CameraComponent.generated.h>
#include <Components/CollisionComponent.generated.h>
#include <Components/LightComponent.generated.h>
#include <Components/MovementComponent.generated.h>
#include <Components/RenderComponent.generated.h>
#include <Components/TransformComponent.generated.h>

//============================================================================
MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::mainwindow)
{
	ui->setupUi(this);

	registerFactories();

	initCommandStack();

	initDockManager();

	fillWindowContent();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::registerFactories()
{
	ComponentRegistration::RegisterComponentFactory(mComponentFactory);
	mComponentContentFactory.registerComponents();
}

void MainWindow::initCommandStack()
{
	mCommandStack.bindFunctionToCommandChange([this](bool needToReloadLayout)
	{
		this->updateUndoRedo();
		if (needToReloadLayout)
		{
			this->updateSelectedComponentData();
		}
	});

	updateUndoRedo();
}

void MainWindow::initDockManager()
{
	mDockManager = std::make_unique<ads::CDockManager>(this);
}

void MainWindow::fillWindowContent()
{
	createWorldSettingsToolbox();
	createEntityListToolbox();
	createComponentListToolbox();
	createComponentAttributesToolbox();
}

void MainWindow::createWorldSettingsToolbox()
{
	if (ads::CDockWidget* dockWidget = mDockManager->findDockWidget("WorldSettingsToolbox"))
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
	ads::CDockWidget* dockWidget = new ads::CDockWidget(QString("World Settings"));
	dockWidget->setObjectName("WorldSettingsToolbox");
	dockWidget->setWidget(containerWidget);
	dockWidget->setToggleViewActionMode(ads::CDockWidget::ActionModeShow);
	dockWidget->setIcon(this->style()->standardIcon(QStyle::SP_DialogOpenButton));
	dockWidget->setFeature(ads::CDockWidget::DockWidgetClosable, true);
	mDockManager->addDockWidget(ads::CenterDockWidgetArea, dockWidget);

	containerWidget->setObjectName("WorldSettingsContainer");

	QVBoxLayout* layout = new QVBoxLayout();
	containerWidget->setLayout(layout);
	QWidget* content = new QWidget();
	content->setObjectName("WorldSettingsContainerContent");
	layout->addWidget(content);
}

void MainWindow::createEntityListToolbox()
{
	if (ads::CDockWidget* dockWidget = mDockManager->findDockWidget("EntityListToolbox"))
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
	ads::CDockWidget* dockWidget = new ads::CDockWidget(QString("World Entities"));
	dockWidget->setObjectName("EntityListToolbox");
	dockWidget->setWidget(containerWidget);
	dockWidget->setToggleViewActionMode(ads::CDockWidget::ActionModeShow);
	dockWidget->setIcon(this->style()->standardIcon(QStyle::SP_DialogOpenButton));
	dockWidget->setFeature(ads::CDockWidget::DockWidgetClosable, true);
	mDockManager->addDockWidget(ads::CenterDockWidgetArea, dockWidget);

	containerWidget->setObjectName("EntityListContainer");

	QVBoxLayout* layout = new QVBoxLayout();
	containerWidget->setLayout(layout);
	QListWidget* entityList = new QListWidget();
	layout->addWidget(entityList);
	entityList->setObjectName("EntityList");

	QObject::connect(entityList, &QListWidget::currentItemChanged, this, &MainWindow::on_entitiesList_currentItemChanged);
}

void MainWindow::createComponentListToolbox()
{
	if (ads::CDockWidget* dockWidget = mDockManager->findDockWidget("ComponentListToolbox"))
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
	ads::CDockWidget* dockWidget = new ads::CDockWidget(QString("Entity Components"));
	dockWidget->setObjectName("ComponentListToolbox");
	dockWidget->setWidget(containerWidget);
	dockWidget->setToggleViewActionMode(ads::CDockWidget::ActionModeShow);
	dockWidget->setIcon(this->style()->standardIcon(QStyle::SP_DialogOpenButton));
	dockWidget->setFeature(ads::CDockWidget::DockWidgetClosable, true);
	mDockManager->addDockWidget(ads::CenterDockWidgetArea, dockWidget);

	containerWidget->setObjectName("ComponentListContainer");

	QVBoxLayout* layout = new QVBoxLayout();
	containerWidget->setLayout(layout);
	QListWidget* componentList = new QListWidget();
	layout->addWidget(componentList);
	componentList->setObjectName("ComponentList");

	QObject::connect(componentList, &QListWidget::currentItemChanged, this, &MainWindow::on_componentsList_currentItemChanged);
}

void MainWindow::createComponentAttributesToolbox()
{
	if (ads::CDockWidget* dockWidget = mDockManager->findDockWidget("ComponentAttributesToolbox"))
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
	dockWidget->setObjectName("ComponentAttributesToolbox");
	dockWidget->setWidget(containerWidget);
	dockWidget->setToggleViewActionMode(ads::CDockWidget::ActionModeShow);
	dockWidget->setIcon(this->style()->standardIcon(QStyle::SP_DialogOpenButton));
	dockWidget->setFeature(ads::CDockWidget::DockWidgetClosable, true);
	mDockManager->addDockWidget(ads::CenterDockWidgetArea, dockWidget);

	containerWidget->setObjectName("ComponentAttributesContainer");

	QVBoxLayout* layout = new QVBoxLayout();
	containerWidget->setLayout(layout);
}

void MainWindow::on_actionNew_World_triggered()
{
	createWorld();
	updateWorldData();
	mOpenedWorldPath.clear();
	ui->actionSave_World->setEnabled(false);
}

void MainWindow::on_actionOpen_World_triggered()
{
	std::string fileName = QFileDialog::getOpenFileName(this,
		tr("Open World"), "resources", tr("Stealth Game World Files (*.json)")).toStdString();

	if (fileName.empty())
	{
		return;
	}

	createWorld();
	WorldLoader::LoadWorld(*mCurrentWorld.get(), fileName, mComponentFactory);
	updateWorldData();
	mOpenedWorldPath = fileName;
	ui->actionSave_World->setEnabled(true);
}

void MainWindow::createWorld()
{
	mCurrentWorld = std::make_unique<World>();
	mCommandStack.clear();
	ui->actionRun_Game->setEnabled(true);
}

void MainWindow::updateWorldData()
{
	QStringList entitiesStringList;
	const auto& entities = mCurrentWorld->getEntityManger().getEntities();
	for (auto& entity : entities)
	{
		entitiesStringList.append(QString::number(entity.first));
	}

	if (QListWidget* entitiesList = mDockManager->findChild<QListWidget*>("EntityList"))
	{
		entitiesList->clear();
		entitiesList->addItems(entitiesStringList);
	}

	updateWorldSettingsContent(entitiesStringList);
}

void MainWindow::updateWorldSettingsContent(const QStringList& entitiesStringList)
{
	QWidget* worldSettingsContainerWidget = mDockManager->findChild<QWidget*>("WorldSettingsContainer");
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
		if (NullableEntity controlledEntity = mCurrentWorld->getPlayerControlledEntity(); controlledEntity.isValid())
		{
			controlledEntityCombobox->setCurrentText(QString::number(controlledEntity.mId));
		}
		QObject::connect(controlledEntityCombobox, &QComboBox::currentTextChanged, this, &MainWindow::on_controlledEntityCombobox_currentIndexChanged);
	}

	{
		worldSettingsLayout->addWidget(new QLabel("Camera Entity"));
		QComboBox* cameraEntityCombobox = new QComboBox();
		cameraEntityCombobox->setObjectName("CameraEntityCombobox");
		worldSettingsLayout->addWidget(cameraEntityCombobox);
		cameraEntityCombobox->addItems(entitiesStringList);
		if (NullableEntity camera = mCurrentWorld->getMainCamera(); camera.isValid())
		{
			cameraEntityCombobox->setCurrentText(QString::number(camera.mId));
		}
		QObject::connect(cameraEntityCombobox, &QComboBox::currentTextChanged, this, &MainWindow::on_cameraEntityCombobox_currentIndexChanged);
	}

	worldSettingsLayout->addStretch();

	QWidget* oldContent = worldSettingsContainerWidget->findChild<QWidget*>("WorldSettingsContainerContent");
	content->setObjectName("WorldSettingsContainerContent");
	worldSettingsContainerWidget->layout()->replaceWidget(oldContent, content);
	delete oldContent;
}

void MainWindow::updateSelectedEntityComponents(QListWidgetItem* selectedItem)
{
	QListWidget* componentsList = mDockManager->findChild<QListWidget*>("ComponentList");

	if (componentsList == nullptr)
	{
		return;
	}

	componentsList->clear();

	if (selectedItem && mCurrentWorld)
	{
		unsigned int entityUid = selectedItem->text().toUInt();
		std::vector<BaseComponent*> components = mCurrentWorld->getEntityManger().getAllEntityComponents(Entity(entityUid));
		for (auto& component : components)
		{
			componentsList->addItem(QString::fromStdString(component->getComponentTypeName()));
		}
	}
}

void MainWindow::updateSelectedComponentData(QListWidgetItem* selectedItem)
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

	bool ok;
	int index = currentItem->text().toInt(&ok);
	if (ok && index != 0)
	{
		Entity entity = Entity(static_cast<Entity::EntityID>(index));
		std::vector<BaseComponent*> entityComponents = mCurrentWorld->getEntityManger().getAllEntityComponents(entity);
		auto it = std::find_if(entityComponents.begin(), entityComponents.end(), [name = selectedItem->text().toStdString()](BaseComponent* component)
		{
			return component->getComponentTypeName().compare(name) == 0;
		});

		if (it != entityComponents.end())
		{
			mComponentContentFactory.replaceEditContent(componentAttributesContainerWidget->layout(), entity, *it, mCommandStack, mCurrentWorld.get());
			validComponentIsSelected = true;
		}
	}

	if (!validComponentIsSelected)
	{
		mComponentContentFactory.removeEditContent(componentAttributesContainerWidget->layout());
	}
}

void MainWindow::updateSelectedComponentData()
{
	if (QListWidget* componentsList = mDockManager->findChild<QListWidget*>("ComponentList"))
	{
		if (QListWidgetItem* currentItem = componentsList->currentItem())
		{
			updateSelectedComponentData(currentItem);
		}
	}
}

void MainWindow::updateUndoRedo()
{
	ui->actionUndo->setEnabled(mCommandStack.haveSomethingToUndo());
	ui->actionRedo->setEnabled(mCommandStack.haveSomethingToRedo());
}

void MainWindow::on_controlledEntityCombobox_currentIndexChanged(const QString &arg1)
{
	if (mCurrentWorld == nullptr)
	{
		return;
	}

	NullableEntity entity;
	int index = arg1.toInt();
	if (index != 0)
	{
		entity = Entity(static_cast<Entity::EntityID>(index));
	}

	QComboBox* controlledEntityCombobox = mDockManager->findChild<QComboBox*>("ControlledEntityCombobox");
	mCommandStack.executeNewCommand<ChangeEntityCommand>(mCurrentWorld.get(),
		&World::setPlayerControlledEntity,
		mCurrentWorld->getPlayerControlledEntity(),
		entity,
		controlledEntityCombobox);
}

void MainWindow::on_cameraEntityCombobox_currentIndexChanged(const QString &arg1)
{
	if (mCurrentWorld == nullptr)
	{
		return;
	}

	NullableEntity entity;
	int index = arg1.toInt();
	if (index != 0)
	{
		entity = Entity(static_cast<Entity::EntityID>(index));
	}

	QComboBox* cameraEntityCombobox = mDockManager->findChild<QComboBox*>("CameraEntityCombobox");
	mCommandStack.executeNewCommand<ChangeEntityCommand>(mCurrentWorld.get(),
		&World::setMainCamera,
		mCurrentWorld->getMainCamera(),
		entity,
		cameraEntityCombobox);
}

void MainWindow::on_actionSave_World_As_triggered()
{
	std::string fileName = QFileDialog::getSaveFileName(this,
		tr("Save World"), "resources", tr("Stealth Game World Files (*.json)")).toStdString();

	if (fileName.empty())
	{
		return;
	}

	WorldLoader::SaveWorld(*mCurrentWorld.get(), fileName, mComponentFactory);
	mOpenedWorldPath = fileName;
	ui->actionSave_World->setEnabled(true);
}

void MainWindow::on_actionSave_World_triggered()
{
	if (mCurrentWorld == nullptr)
	{
		return;
	}

	WorldLoader::SaveWorld(*mCurrentWorld.get(), mOpenedWorldPath, mComponentFactory);
}

void MainWindow::on_actionRun_Game_triggered()
{
	if (!QDir("./tmp").exists())
	{
		QDir().mkdir("./tmp");
	}
	static std::string tempWorldName = "./tmp/temp-editor-world.json";
	WorldLoader::SaveWorld(*mCurrentWorld.get(), tempWorldName, mComponentFactory);
	QProcess::startDetached("StealthGamePrototype", {"--world", QString::fromStdString(tempWorldName)});
}

void MainWindow::on_actionUndo_triggered()
{
	if (mCurrentWorld)
	{
		mCommandStack.undo(mCurrentWorld.get());
	}
}

void MainWindow::on_actionRedo_triggered()
{
	if (mCurrentWorld)
	{
		mCommandStack.redo(mCurrentWorld.get());
	}
}

void MainWindow::on_entitiesList_currentItemChanged(QListWidgetItem* current, QListWidgetItem* /*previous*/)
{
	updateSelectedEntityComponents(current);
}

void MainWindow::on_componentsList_currentItemChanged(QListWidgetItem* current, QListWidgetItem* /*previous*/)
{
	updateSelectedComponentData(current);
}

void MainWindow::on_actionCreate_triggered()
{
	if (mCurrentWorld)
	{
		mCurrentWorld->getEntityManger().addEntity();
	}
	updateWorldData();
}

void MainWindow::on_actionEntities_List_triggered()
{
	createEntityListToolbox();
}

void MainWindow::on_actionComponents_List_triggered()
{
	createComponentListToolbox();
}

void MainWindow::on_actionComponent_Properties_triggered()
{
	createComponentAttributesToolbox();
}

void MainWindow::on_actionWorld_Settings_triggered()
{
	createWorldSettingsToolbox();
}
