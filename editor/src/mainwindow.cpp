#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "src/editorcommands/changeentitycommand.h"

#include <Core/World.h>
#include <Modules/WorldLoader.h>

#include <QFileDialog>

#include <Components/CameraComponent.h>
#include <Components/CollisionComponent.h>
#include <Components/LightComponent.h>
#include <Components/MovementComponent.h>
#include <Components/RenderComponent.h>
#include <Components/TransformComponent.h>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	mComponentFactory.registerComponent<CameraComponent>();
	mComponentFactory.registerComponent<CollisionComponent>();
	mComponentFactory.registerComponent<LightComponent>();
	mComponentFactory.registerComponent<MovementComponent>();
	mComponentFactory.registerComponent<RenderComponent>();
	mComponentFactory.registerComponent<TransformComponent>();

	mCommandStack.bindFunctionToCommandChange([this]()
	{
		this->updateUndoRedo();
	});
	updateUndoRedo();
}

MainWindow::~MainWindow()
{
	delete ui;
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

	fileName = fileName.substr(0, fileName.rfind(".json"));

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
}

void MainWindow::updateWorldData()
{
	QStringList entitiesStringList;
	const auto& entities = mCurrentWorld->getEntityManger().getEntities();
	for (auto& entity : entities)
	{
		entitiesStringList.append(QString::number(entity.first));
	}

	QListWidget* entitiesList = ui->centralWidget->findChild<QListWidget*>("entitiesList");
	entitiesList->clear();
	entitiesList->addItems(entitiesStringList);

	QComboBox* controlledEntityCombobox = ui->centralWidget->findChild<QComboBox*>("controlledEntityCombobox");
	controlledEntityCombobox->blockSignals(true);
	controlledEntityCombobox->clear();
	controlledEntityCombobox->addItems(entitiesStringList);
	if (NullableEntity controlledEntity = mCurrentWorld->getPlayerControlledEntity(); controlledEntity.isValid())
	{
		controlledEntityCombobox->setCurrentText(QString::number(controlledEntity.mId));
	}
	controlledEntityCombobox->blockSignals(false);

	QComboBox* cameraEntityCombobox = ui->centralWidget->findChild<QComboBox*>("cameraEntityCombobox");
	cameraEntityCombobox->blockSignals(true);
	cameraEntityCombobox->clear();
	cameraEntityCombobox->addItems(entitiesStringList);
	if (NullableEntity camera = mCurrentWorld->getMainCamera(); camera.isValid())
	{
		cameraEntityCombobox->setCurrentText(QString::number(camera.mId));
	}
	cameraEntityCombobox->blockSignals(false);

	updateSelectedEntityComponents();
}

void MainWindow::updateSelectedEntityComponents()
{
	updateSelectedComponentData();
}

void MainWindow::updateSelectedComponentData()
{

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

	QComboBox* controlledEntityCombobox = ui->centralWidget->findChild<QComboBox*>("controlledEntityCombobox");
	mCommandStack.executeNewCommand<ChangeEntityCommand>(mCurrentWorld.get(),
														 this,
														 &World::setPlayerControlledEntity,
														 entity,
														 mCurrentWorld->getPlayerControlledEntity(),
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

	QComboBox* cameraEntityCombobox = ui->centralWidget->findChild<QComboBox*>("cameraEntityCombobox");
	mCommandStack.executeNewCommand<ChangeEntityCommand>(mCurrentWorld.get(),
														 this,
														 &World::setMainCamera,
														 entity,
														 mCurrentWorld->getMainCamera(),
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

	fileName = fileName.substr(0, fileName.rfind(".json"));

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

}

void MainWindow::on_actionUndo_triggered()
{
	if (mCurrentWorld)
	{
		mCommandStack.undo(mCurrentWorld.get(), this);
	}
}

void MainWindow::on_actionRedo_triggered()
{
	if (mCurrentWorld)
	{
		mCommandStack.redo(mCurrentWorld.get(), this);
	}
}
