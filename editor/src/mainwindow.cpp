#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "src/editorcommands/changeentitycommand.h"
#include "src/componenteditcontent/componentcontentfactory.h"

#include "src/componenteditcontent/componentregistration.h"

#include <Core/World.h>
#include <Modules/WorldLoader.h>

#include <QFileDialog>
#include <QProcess>

#include <Components/CameraComponent.h>
#include <Components/CollisionComponent.h>
#include <Components/LightComponent.h>
#include <Components/MovementComponent.h>
#include <Components/RenderComponent.h>
#include <Components/TransformComponent.h>

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ComponentRegistration::RegisterComponentFactory(mComponentFactory);
	mComponentContentFactory.registerComponents();

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
}

void MainWindow::updateSelectedEntityComponents(QListWidgetItem* selectedItem)
{
	QListWidget* componentsList = ui->componentsList;
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
	if (selectedItem != nullptr)
	{
		QListWidgetItem* currentItem = ui->entitiesList->currentItem();
		if (currentItem != nullptr)
		{
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
					mComponentContentFactory.replaceEditContent(ui->componentPropertiesContainer->layout(), entity, *it, mCommandStack, mCurrentWorld.get());
					validComponentIsSelected = true;
				}
			}
		}
	}

	if (!validComponentIsSelected)
	{
		mComponentContentFactory.removeEditContent(ui->componentPropertiesContainer->layout());
	}
}

void MainWindow::updateSelectedComponentData()
{
	QListWidgetItem* currentItem = ui->componentsList->currentItem();
	if (currentItem)
	{
		updateSelectedComponentData(currentItem);
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

	QComboBox* controlledEntityCombobox = ui->centralWidget->findChild<QComboBox*>("controlledEntityCombobox");
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

	QComboBox* cameraEntityCombobox = ui->centralWidget->findChild<QComboBox*>("cameraEntityCombobox");
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
	updateSelectedComponentData();
}

void MainWindow::on_actionRedo_triggered()
{
	if (mCurrentWorld)
	{
		mCommandStack.redo(mCurrentWorld.get());
	}
	updateSelectedComponentData();
}

void MainWindow::on_entitiesList_currentItemChanged(QListWidgetItem* current, QListWidgetItem* /*previous*/)
{
	updateSelectedEntityComponents(current);
}

void MainWindow::on_componentsList_currentItemChanged(QListWidgetItem* current, QListWidgetItem* /*previous*/)
{
	updateSelectedComponentData(current);
}
