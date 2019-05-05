#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QListWidgetItem>

#include "editorcommands/editorcommandsstack.h"
#include "componenteditcontent/componentcontentfactory.h"

#include <Modules/ComponentFactory.h>

namespace Ui {
	class mainwindow;
}

namespace ads
{
	class CDockManager;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

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
	void initDockManager();
	void initCommandStack();
	void registerFactories();
	void fillWindowContent();
	void createWorldSettingsToolbox();
	void createEntityListToolbox();
	void createComponentListToolbox();
	void createComponentAttributesToolbox();
	void createWorld();
	void updateWorldData();
	void updateWorldSettingsContent(const QStringList& entitiesStringList);
	void updateSelectedEntityComponents(QListWidgetItem *selectedItem);
	void updateSelectedComponentData(QListWidgetItem* selectedItem);
	void updateSelectedComponentData();
	void updateUndoRedo();

	void on_controlledEntityCombobox_currentIndexChanged(const QString& arg1);
	void on_cameraEntityCombobox_currentIndexChanged(const QString& arg1);
	void on_entitiesList_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	void on_componentsList_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

private:
	// need to be a raw pointer in order to Qt Designer to work normally with this class
	Ui::mainwindow* ui;
	std::unique_ptr<ads::CDockManager> mDockManager;
	std::unique_ptr<class World> mCurrentWorld;
	ComponentFactory mComponentFactory;
	ComponentContentFactory mComponentContentFactory;
	std::string mOpenedWorldPath;
	EditorCommandsStack mCommandStack;
};

#endif // MAINWINDOW_H
