#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "src/componenteditcontent/componentcontentfactory.h"

#include "src/editorcommands/addentitycommand.h"
#include "src/editorcommands/addspatialentitycommand.h"
#include "src/editorutils/worldsaveutils.h"

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"

#include "GameData/ComponentRegistration/ComponentFactoryRegistration.h"
#include "GameData/ComponentRegistration/ComponentJsonSerializerRegistration.h"
#include "GameData/World.h"

#include "Utils/World/GameDataLoader.h"

#include "toolboxes/ComponentAttributesToolbox.h"
#include "toolboxes/ComponentsListToolbox.h"
#include "toolboxes/EntitiesListToolbox.h"
#include "toolboxes/PrefabListToolbox.h"
#include "toolboxes/TransformEditorToolbox.h"

#include <QFileDialog>
#include <QProcess>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(HS_NEW Ui::mainwindow)
{
	ui->setupUi(this);

	registerFactories();

	initCommandStack();

	initToolboxes();

	fillWindowContent();

	initActions();

	bindEvents();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::registerFactories()
{
	ComponentsRegistration::RegisterComponents(mComponentSerializationHolder.factory);
	ComponentsRegistration::RegisterJsonSerializers(mComponentSerializationHolder.jsonSerializer);
	mComponentContentFactory.registerComponents();
}

void MainWindow::initCommandStack()
{
	mCommandStack.bindFunctionToCommandChange([this](EditorCommand::EffectBitset effects, bool originalCall)
	{
		this->updateUndoRedo();
		OnCommandEffectApplied.broadcast(effects, originalCall);
	});

	updateUndoRedo();
}

void MainWindow::initToolboxes()
{
	mDockManager = std::make_unique<ads::CDockManager>(this);
	mEntitiesListToolbox = std::make_unique<EntitiesListToolbox>(this, mDockManager.get());
	mComponentAttributesToolbox = std::make_unique<ComponentAttributesToolbox>(this, mDockManager.get());
	mComponentsListToolbox = std::make_unique<ComponentsListToolbox>(this, mDockManager.get());
	mPrefabListToolbox = std::make_unique<PrefabListToolbox>(this, mDockManager.get());
	mTransformEditorToolbox = std::make_unique<TransformEditorToolbox>(this, mDockManager.get());
}

void MainWindow::fillWindowContent()
{
	mEntitiesListToolbox->show();
	mComponentsListToolbox->show();
	mComponentAttributesToolbox->show();
	mTransformEditorToolbox->show();
}

void MainWindow::createWorld()
{
	mCurrentWorld = std::make_unique<World>();
	mCommandStack.clear();
	ui->actionRun_Game->setEnabled(true);
}

void MainWindow::updateUndoRedo()
{
	ui->actionUndo->setEnabled(mCommandStack.haveSomethingToUndo());
	ui->actionRedo->setEnabled(mCommandStack.haveSomethingToRedo());
}

void MainWindow::initActions()
{
	connect(ui->actionPrefabs, &QAction::triggered, this, &MainWindow::actionPrefabsTriggered);
	connect(ui->actionNew_Prefab_Library, &QAction::triggered, this, &MainWindow::actionNewPrefabLibraryTriggered);
	connect(ui->actionLoad_Prefab_Library, &QAction::triggered, this, &MainWindow::actionLoadPrefabLibraryTriggered);
	connect(ui->actionSave_Prefab_Library, &QAction::triggered, this, &MainWindow::actionSavePrefabLibraryTriggered);
	connect(ui->actionSave_Prefab_Library_As, &QAction::triggered, this, &MainWindow::actionSavePrefabLibraryAsTriggered);
}

void MainWindow::bindEvents()
{
	// on selected entity change broadcast selected component source change automatically
	OnSelectedEntityChanged.bind([this](const std::optional<EntityReference>& ref)
	{
		if (ref.has_value())
		{
			ComponentSourceReference componentSourceReference;
			componentSourceReference.entity = ref->entity;
			componentSourceReference.cellPos = ref->cellPos;
			componentSourceReference.isWorld = true;
			OnSelectedComponentSourceChanged.broadcast(componentSourceReference);
		}
		else
		{
			OnSelectedComponentSourceChanged.broadcast(std::nullopt);
		}
	});
}

void MainWindow::actionPrefabsTriggered()
{
	mPrefabListToolbox->show();
}

void MainWindow::actionNewPrefabLibraryTriggered()
{
	mPrefabListToolbox->clear();
}

void MainWindow::actionLoadPrefabLibraryTriggered()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Load Prefab"), "../resources/prefabs", tr("Prefab Files (*.json)"));

	if (fileName.isEmpty())
	{
		return;
	}

	mPrefabListToolbox->loadFromFile(fileName);
}

void MainWindow::actionSavePrefabLibraryTriggered()
{
	mPrefabListToolbox->saveToFile();
}

void MainWindow::actionSavePrefabLibraryAsTriggered()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Prefab"), "../resources/prefabs", tr("Prefab Files (*.json)"));

	if (fileName.isEmpty())
	{
		return;
	}

	mPrefabListToolbox->saveToFile(fileName);
}

void MainWindow::on_actionNew_World_triggered()
{
	createWorld();
	mOpenedWorldPath.clear();
	ui->actionSave_World->setEnabled(false);
	ui->actionCreate->setEnabled(true);
	ui->actionCreate_Spatial->setEnabled(true);

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
	GameDataLoader::LoadWorld(*mCurrentWorld.get(), fileName, mComponentSerializationHolder);
	mOpenedWorldPath = fileName;
	ui->actionSave_World->setEnabled(true);
	ui->actionCreate->setEnabled(true);
	ui->actionCreate_Spatial->setEnabled(true);

	OnWorldChanged.broadcast();
}

void MainWindow::on_actionSave_World_As_triggered()
{
	std::string fileName = QFileDialog::getSaveFileName(this,
		tr("Save World"), "../resources/maps", tr("Stealth Game World Files (*.json)")).toStdString();

	if (fileName.empty())
	{
		return;
	}

	Utils::SaveWorld(*mCurrentWorld.get(), fileName, mComponentSerializationHolder);
	mOpenedWorldPath = fileName;
	ui->actionSave_World->setEnabled(true);
}

void MainWindow::on_actionSave_World_triggered()
{
	if (mCurrentWorld == nullptr)
	{
		return;
	}


	Utils::SaveWorld(*mCurrentWorld.get(), mOpenedWorldPath, mComponentSerializationHolder);
}

void MainWindow::on_actionRun_Game_triggered()
{
	if (!QDir("./tmp").exists())
	{
		QDir().mkdir("./tmp");
	}
	static std::string tempWorldName = "./tmp/temp-editor-world.json";
	Utils::SaveWorld(*mCurrentWorld.get(), tempWorldName, mComponentSerializationHolder);
	QProcess::startDetached("./GameMain", {"--world", QString::fromStdString(tempWorldName)});
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
		mCommandStack.executeNewCommand<AddEntityCommand>(mCurrentWorld.get(), mCurrentWorld->getEntityManager().getNonExistentEntity());
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

void MainWindow::on_actionTransform_Editor_triggered()
{
	mTransformEditorToolbox->show();
}

void MainWindow::on_actionEdit_Components_triggered()
{
	ComponentSourceReference source;
	source.isWorld = true;
	OnSelectedComponentSourceChanged.broadcast(source);
}

void MainWindow::on_actionCreate_Spatial_triggered()
{
	if (mCurrentWorld)
	{
		SpatialEntity entity{mCurrentWorld->getEntityManager().getNonExistentEntity(), CellPos(0, 0)};
		Vector2D location{ZERO_VECTOR};
		if (mTransformEditorToolbox->isShown())
		{
			 std::tie(entity.cell, location) = mTransformEditorToolbox->getWidgetCenterWorldPosition();
		}
		mCommandStack.executeNewCommand<AddSpatialEntityCommand>(mCurrentWorld.get(), entity, location);
	}
}
