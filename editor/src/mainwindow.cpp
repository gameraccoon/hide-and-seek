#include "mainwindow.h"
#include "ui_mainwindow.h"

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
	controlledEntityCombobox->clear();
	controlledEntityCombobox->addItems(entitiesStringList);
	if (NullableEntity controlledEntity = mCurrentWorld->getPlayerControlledEntity(); controlledEntity.isValid())
	{
		controlledEntityCombobox->setCurrentText(QString::number(controlledEntity.mId));
	}

	QComboBox* cameraEntityCombobox = ui->centralWidget->findChild<QComboBox*>("cameraEntityCombobox");
	cameraEntityCombobox->clear();
	cameraEntityCombobox->addItems(entitiesStringList);
	if (NullableEntity camera = mCurrentWorld->getMainCamera(); camera.isValid())
	{
		cameraEntityCombobox->setCurrentText(QString::number(camera.mId));
	}

	updateSelectedEntityComponents();
}

void MainWindow::updateSelectedEntityComponents()
{
	updateSelectedComponentData();
}

void MainWindow::updateSelectedComponentData()
{

}

void MainWindow::on_controlledEntityCombobox_currentIndexChanged(const QString &arg1)
{

}

void MainWindow::on_cameraEntityCombobox_currentIndexChanged(const QString &arg1)
{

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

}

void MainWindow::on_actionRedo_triggered()
{

}
