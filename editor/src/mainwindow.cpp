#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "src/componenteditcontent/componentcontentfactory.h"

#include "src/componenteditcontent/componentregistration.h"

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include <Core/World.h>
#include <Modules/WorldLoader.h>

#include "toolboxes/ComponentAttributesToolbox.h"
#include "toolboxes/ComponentsListToolbox.h"
#include "toolboxes/EntitiesListToolbox.h"
#include "toolboxes/WorldPropertiesToolbox.h"

#include <QFileDialog>
#include <QProcess>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::mainwindow)
{
	ui->setupUi(this);

	registerFactories();

	initCommandStack();

	initToolboxes();

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

void MainWindow::initToolboxes()
{
	mDockManager = std::make_unique<ads::CDockManager>(this);
	mEntitiesListToolbox = std::make_unique<EntitiesListToolbox>(this, mDockManager.get());
	mComponentAttributesToolbox = std::make_unique<ComponentAttributesToolbox>(this, mDockManager.get());
	mComponentsListToolbox = std::make_unique<ComponentsListToolbox>(this, mDockManager.get());
	mWorldPropertiesToolbox = std::make_unique<WorldPropertiesToolbox>(this, mDockManager.get());
}

void MainWindow::fillWindowContent()
{
	mEntitiesListToolbox->show();
	mComponentsListToolbox->show();
	mComponentAttributesToolbox->show();
	mWorldPropertiesToolbox->show();
}

void MainWindow::on_actionNew_World_triggered()
{
	createWorld();
	mOpenedWorldPath.clear();
	ui->actionSave_World->setEnabled(false);

	OnWorldChanged.broadcast();
}

void MainWindow::on_actionOpen_World_triggered()
{
	std::string fileName = QFileDialog::getOpenFileName(this,
		tr("Open World"), "../resources/maps", tr("Stealth Game World Files (*.json)")).toStdString();

	if (fileName.empty())
	{
		return;
	}

	createWorld();
	WorldLoader::LoadWorld(*mCurrentWorld.get(), fileName, mComponentFactory);
	mOpenedWorldPath = fileName;
	ui->actionSave_World->setEnabled(true);

	OnWorldChanged.broadcast();
}

void MainWindow::createWorld()
{
	mCurrentWorld = std::make_unique<World>();
	mCommandStack.clear();
	ui->actionRun_Game->setEnabled(true);
}

void MainWindow::updateSelectedComponentData()
{
	if (QListWidget* componentsList = mDockManager->findChild<QListWidget*>(ComponentsListToolbox::ListName))
	{
		if (QListWidgetItem* currentItem = componentsList->currentItem())
		{
			mComponentsListToolbox->updateSelectedComponentData(currentItem);
		}
	}
}

void MainWindow::updateUndoRedo()
{
	ui->actionUndo->setEnabled(mCommandStack.haveSomethingToUndo());
	ui->actionRedo->setEnabled(mCommandStack.haveSomethingToRedo());
}

void MainWindow::on_actionSave_World_As_triggered()
{
	std::string fileName = QFileDialog::getSaveFileName(this,
		tr("Save World"), "../resources/maps", tr("Stealth Game World Files (*.json)")).toStdString();

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

void MainWindow::on_actionCreate_triggered()
{
	if (mCurrentWorld)
	{
		mCurrentWorld->getEntityManger().addEntity();
	}
}

void MainWindow::on_actionEntities_List_triggered()
{
	mEntitiesListToolbox->show();
}

void MainWindow::on_actionComponents_List_triggered()
{
	mComponentsListToolbox->show();
}

void MainWindow::on_actionComponent_Properties_triggered()
{
	mComponentAttributesToolbox->show();
}

void MainWindow::on_actionWorld_Settings_triggered()
{
	mWorldPropertiesToolbox->show();
}
