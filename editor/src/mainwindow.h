#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QListWidgetItem>

#include "editorcommands/editorcommandsstack.h"
#include "componenteditcontent/componentcontentfactory.h"

#include "ECS/Delegates.h"
#include "ECS/Serialization/ComponentSerializersHolder.h"

#include "GameData/Spatial/SpatialEntity.h"

#include "editorutils/componentreference.h"
#include "editorutils/entityreference.h"

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
class PrefabListToolbox;
class TransformEditorToolbox;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	World* getCurrentWorld() { return mCurrentWorld.get(); }
	const ComponentSerializersHolder& getComponentSerializationHolder() const { return mComponentSerializationHolder; }
	ComponentContentFactory& getComponentContentFactory() { return mComponentContentFactory; }
	EditorCommandsStack& getCommandStack() { return mCommandStack; }
	PrefabListToolbox* getPrefabToolbox() { return mPrefabListToolbox.get(); }

public:
	MulticastDelegate<> OnWorldChanged;
	MulticastDelegate<const std::optional<EntityReference>&> OnSelectedEntityChanged;
	MulticastDelegate<const std::optional<ComponentSourceReference>&> OnSelectedComponentSourceChanged;
	MulticastDelegate<const std::optional<ComponentReference>&> OnSelectedComponentChanged;
	MulticastDelegate<EditorCommand::EffectBitset, bool> OnCommandEffectApplied;

private slots:
	void on_actionTransform_Editor_triggered();

private:
	void initToolboxes();
	void initCommandStack();
	void registerFactories();
	void fillWindowContent();
	void createWorld();
	void updateSelectedComponentData(QListWidgetItem* selectedItem);
	void updateUndoRedo();
	void initActions();
	void bindEvents();

	void actionPrefabsTriggered();
	void actionNewPrefabLibraryTriggered();
	void actionLoadPrefabLibraryTriggered();
	void actionSavePrefabLibraryTriggered();
	void actionSavePrefabLibraryAsTriggered();

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

	void on_actionEdit_Components_triggered();

	void on_actionCreate_Spatial_triggered();

private:
	// need to be a raw pointer in order to Qt Designer to work normally with this class
	Ui::mainwindow* ui;
	std::unique_ptr<ads::CDockManager> mDockManager;
	std::unique_ptr<class World> mCurrentWorld;
	ComponentSerializersHolder mComponentSerializationHolder;
	ComponentContentFactory mComponentContentFactory;
	std::string mOpenedWorldPath;
	EditorCommandsStack mCommandStack;

	std::unique_ptr<EntitiesListToolbox> mEntitiesListToolbox;
	std::unique_ptr<ComponentAttributesToolbox> mComponentAttributesToolbox;
	std::unique_ptr<ComponentsListToolbox> mComponentsListToolbox;
	std::unique_ptr<PrefabListToolbox> mPrefabListToolbox;
	std::unique_ptr<TransformEditorToolbox> mTransformEditorToolbox;
};

#endif // MAINWINDOW_H
