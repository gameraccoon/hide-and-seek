#pragma once

#include <QString>
#include <QWidget>

#include "ECS/Delegates.h"
#include "ECS/Entity.h"

#include "src/editorutils/entityreference.h"
#include "src/editorutils/componentreference.h"

class MainWindow;

namespace ads
{
	class CDockManager;
}

class QListWidgetItem;

class ComponentsListToolbox : public QWidget
{
public:
	ComponentsListToolbox(MainWindow* mainWindow, ads::CDockManager* dockManager);
	~ComponentsListToolbox();
	void show();

	static const QString WidgetName;
	static const QString ToolboxName;
	static const QString ContainerName;
	static const QString ContainerContentName;
	static const QString ListName;

private:
	void updateContent();
	void onCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
	void onSelectedComponentSourceChanged(const std::optional<ComponentSourceReference>& newSource);
	void showContextMenu(const QPoint& pos);
	void removeSelectedComponent();

	void bindEvents();
	void unbindEvents();

private:
	MainWindow* mMainWindow;
	ads::CDockManager* mDockManager;
	std::optional<ComponentSourceReference> mLastSelectedComponentSource;

	Delegates::Handle mOnComponentAddedHandle;
	Delegates::Handle mOnComponentRemovedHandle;
	Delegates::Handle mOnWorldChangedHandle;
	Delegates::Handle mOnComponentSourceChangedHandle;
};
