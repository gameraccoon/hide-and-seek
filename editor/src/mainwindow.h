#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QListWidgetItem>

#include "editorcommands/editorcommandsstack.h"
#include "componenteditcontent/componentcontentfactory.h"

#include <Modules/ComponentFactory.h>

namespace ads
{
	class CDockManager;
}

namespace Ui
{
	class mainwindow;
}

class ComponentAttributesToolbox;
class ComponentsListToolbox;
class EntitiesListToolbox;
class WorldPropertiesToolbox;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	World* getCurrentWorld() { return mCurrentWorld.get(); }
	ComponentFactory& getComponentFactory() { return mComponentFactory; }
	ComponentContentFactory& getComponentContentFactory() { return mComponentContentFactory; }
	EditorCommandsStack& getCommandStack() { return mCommandStack; }

private slots:
	void on_actionNew_World_triggered();
	void on_actionOpen_World_triggered();
	void on_actionSave_World_As_triggered();
	void on_actionSave_World_triggered();
	void on_actionRun_Game_triggered();
	void on_actionUndo_triggered();
	void on_actionRedo_triggered();
	void on_actionCreate_triggered();
	void on_actionEntities_List_triggered();
	void on_actionComponents_List_triggered();
	void on_actionComponent_Properties_triggered();
	void on_actionWorld_Settings_triggered();

private:
	void initToolboxes();
	void initCommandStack();
	void registerFactories();
	void fillWindowContent();
	void createWorld();
	void updateWorldData();
	void updateSelectedComponentData(QListWidgetItem* selectedItem);
	void updateSelectedComponentData();
	void updateUndoRedo();

private:
	// need to be a raw pointer in order to Qt Designer to work normally with this class
	Ui::mainwindow* ui;
	std::unique_ptr<ads::CDockManager> mDockManager;
	std::unique_ptr<class World> mCurrentWorld;
	ComponentFactory mComponentFactory;
	ComponentContentFactory mComponentContentFactory;
	std::string mOpenedWorldPath;
	EditorCommandsStack mCommandStack;
	std::unique_ptr<EntitiesListToolbox> mEntitiesListToolbox;
	std::unique_ptr<WorldPropertiesToolbox> mWorldPropertiesToolbox;
	std::unique_ptr<ComponentAttributesToolbox> mComponentAttributesToolbox;
	std::unique_ptr<ComponentsListToolbox> mComponentsListToolbox;
};

#endif // MAINWINDOW_H
