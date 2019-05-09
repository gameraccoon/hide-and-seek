#pragma once

#include <QString>
#include <QWidget>

#include "Core/Delegates.h"
#include "Core/Entity.h"

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
	void onSelectedEntityChanged(NullableEntity newEntity);
	void showContextMenu(const QPoint& pos);
	void removeSelectedComponent();

	void bindEvents();
	void unbindEvents();

private:
	MainWindow* mMainWindow;
	ads::CDockManager* mDockManager;
	NullableEntity mLastSelectedEntity;

	Delegates::HandleType mOnComponentAddedHandle;
	Delegates::HandleType mOnComponentRemovedHandle;
	Delegates::HandleType mOnWorldChangedHandle;
	Delegates::HandleType mOnEntityChangedHandle;
};
